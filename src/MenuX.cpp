#include "MenuX.h"

Menu::Menu(
    Button2 &btn1,
    Button2 &btn2,
    TFT_eSPI &tft,
    MenuItem items[],
    int itemCount, 
    int *offsetY
) : 
    button1(btn1),
    button2(btn2), 
    tft(tft), 
    offsetY(offsetY),
    itemCount(itemCount), 
    currentSelection(0), 
    prevSelection(0),
    menuContext(menuContext),
    menuFlow(menuContext)
{
    this->currentMenuItems = items;
    this->currentMenuItemCount = itemCount;
    for (int i = 0; i < itemCount; i++) {
        this->items[i] = &items[i];
    }
}

Flow<Menu::MenuContext>& Menu::getMenuFlow() {
    return menuFlow;
}

void Menu::updateMenuContext() {
    MenuContext context = { 
        currentMenuItems, 
        currentMenuItemCount, 
        currentMenuItems[currentSelection].label, 
        currentMenuItems[currentSelection].description, 
        currentSubmenuTitle 
    };
    menuContext = context;
}

void Menu::publishMenuContext() {
    menuFlow.setState(menuContext);
}

void Menu::begin() {

    button1.setDebounceTime(DEBOUNCE_TIME);
    button2.setDebounceTime(DEBOUNCE_TIME);
    button1.setLongClickTime(LONG_CLICK_TIME);
    button2.setLongClickTime(LONG_CLICK_TIME);

    button1.setClickHandler(NULL);
    button2.setClickHandler(NULL);
    button1.setLongClickDetectedHandler(NULL);
    button2.setLongClickDetectedHandler(NULL);

    button1.setClickHandler([this](Button2 &btn) { handleButton1Press(); });
    button2.setClickHandler([this](Button2 &btn) { handleButton2Press(); });
    button1.setLongClickDetectedHandler([this](Button2 &btn) { back(); });
    button2.setLongClickDetectedHandler([this](Button2 &btn) { handleButtonLongPress(); });

    tft.setTextSize(MENU_FONT_SIZE);
    tft.setTextFont(MENU_FONT);
    tft.setTextDatum(TL_DATUM);
    
    updateMenuContext();
    publishMenuContext();
    clearMenuField();
    drawMenu();
}

void Menu::navigateUp() {
    if (currentMenuItemCount > 1) {
        if (currentSelection > 0) {
            currentSelection--;
        } else {
            currentSelection = currentMenuItemCount - 1;
        }

        if (currentSelection < scrollOffset) {
            scrollOffset = currentSelection;
        }
        updateMenuContext();
        publishMenuContext();
        drawMenu();
    }
}

void Menu::navigateDown() {
    const int visibleItemCount = (tft.height() - *offsetY - 1) / ROW_HEIGHT;

    if (currentMenuItemCount > 1) {
        if (currentSelection < currentMenuItemCount - 1) {
            currentSelection++;
        } else {
            currentSelection = 0;
        }

        if (currentSelection >= scrollOffset + visibleItemCount) {
            scrollOffset = currentSelection - visibleItemCount + 1;
        }
        updateMenuContext();
        publishMenuContext();
        drawMenu();
    }
}

void Menu::handleButton1Press() {
    navigateDown();
}

void Menu::handleButton2Press() {
    navigateUp();
}

void Menu::select() {
    MenuItem &selectedItem = *items[currentItemIndex];
    if (selectedItem.submenu != nullptr && selectedItem.submenuItemCount > 0) {
        *items = selectedItem.submenu;
        itemCount = selectedItem.submenuItemCount;
        currentItemIndex = 0;
        updateMenuContext();
        publishMenuContext();
    } else if (selectedItem.action != nullptr) {
        selectedItem.action();
    }
}

void Menu::handleButtonLongPress() {
    if (currentSelection < currentMenuItemCount) {
        MenuItem* selectedItem = &currentMenuItems[currentSelection];

        if (selectedItem->submenu != nullptr && selectedItem->submenuItemCount > 0) {
            MenuContext context = { 
                currentMenuItems, 
                currentMenuItemCount, 
                currentMenuItems[currentSelection].label, 
                currentMenuItems[currentSelection].description, 
                currentSubmenuTitle 
            };
            menuContext = context;
            menuStack.push(context);
            
            currentMenuItems = selectedItem->submenu;
            currentMenuItemCount = selectedItem->submenuItemCount;
            currentSelection = 0;
            currentSubmenuTitle = selectedItem->label;

            clearMenuField();
            updateMenuContext();
            publishMenuContext();
            drawMenu();
        }
        else if (selectedItem->action != nullptr) {
            selectedItem->action();
        }
    }
}

void Menu::disableButtons() {
    button1.setClickHandler(NULL);
    button2.setClickHandler(NULL);
    button1.setLongClickDetectedHandler(NULL);
    button2.setLongClickDetectedHandler(NULL);
}

void Menu::enableButtons() {
    begin();
}

void Menu::transitionToInteractor() {
    MenuItem* selectedItem = &currentMenuItems[currentSelection];
    MenuContext context = { 
        currentMenuItems, 
        currentMenuItemCount, 
        currentMenuItems[currentSelection].label, 
        currentMenuItems[currentSelection].description, 
        currentSubmenuTitle 
    };
    menuContext = context;
    menuStack.push(context);
    isMenuActive = false;
    clearMenuField();
    disableButtons();
}

void Menu::returnToMenu() {
    if (!menuStack.empty()) {
        MenuContext previousContext = menuStack.top();
        menuContext = previousContext;
        menuStack.pop();
        currentMenuItems = previousContext.menuItems;
        currentMenuItemCount = previousContext.menuItemCount;
        currentSubmenuTitle = previousContext.currentSectionLabel;
        isMenuActive = true;
        enableButtons();
    }
}

void Menu::clearMenuField() {
    int menuStartY = *offsetY + 1;
    int menuHeight = tft.height() - menuStartY;
    tft.fillRect(0, menuStartY, tft.width(), menuHeight, MENU_BG_COLOR);
}

void Menu::back() {
    if (!menuStack.empty()) {
        MenuContext previousContext = menuStack.top();
        menuStack.pop();
        currentMenuItems = previousContext.menuItems;
        currentMenuItemCount = previousContext.menuItemCount;
        currentSubmenuTitle = previousContext.currentSectionLabel;
        currentSelection = 0;
        clearMenuField();
        drawMenu();
    }
}

void Menu::drawMenu() {
    const int menuStartY = *offsetY + 1;
    const int visibleRows = (tft.height() - menuStartY - ROW_HEIGHT) / ROW_HEIGHT; // Количество видимых строк
    const int endOffset = currentMenuItemCount - visibleRows;

    // Обновляем прокрутку
    if (currentSelection < scrollOffset) {
        scrollOffset = currentSelection; // Прокрутка вверх
    } else if (currentSelection >= scrollOffset + visibleRows) {
        scrollOffset = currentSelection - visibleRows + 1; // Прокрутка вниз
    }

    // Очищаем меню
    clearMenuField();

    // Отрисовка видимых пунктов меню
    for (int i = 0; i < visibleRows && (i + scrollOffset) < currentMenuItemCount; i++) {
        int menuItemIndex = i + scrollOffset;
        int yPos = menuStartY + i * ROW_HEIGHT;

        if (menuItemIndex == currentSelection) {
            tft.fillRect(0, yPos, tft.textWidth(currentMenuItems[menuItemIndex].label) + 18, ROW_HEIGHT, MENU_SEL_COLOR);
        }

        tft.setTextColor(MENU_TEXT_COLOR);
        tft.setCursor(10, yPos + ROW_SHIFT);
        tft.print(currentMenuItems[menuItemIndex].label);
    }

    if (menuStack.size() > 0) {
        int submenuY = tft.height() - ROW_HEIGHT;
        tft.drawLine(0, submenuY - 1, tft.width(), submenuY - 1, MENU_DIVIDER_COLOR);
        tft.fillRect(0, submenuY, tft.width(), ROW_HEIGHT, MENU_BG_COLOR);
        tft.setCursor(2, submenuY + ROW_SHIFT);
        tft.print("<< Back");
        tft.setCursor(tft.width() - tft.textWidth(currentSubmenuTitle) - 10, submenuY + ROW_SHIFT);
        tft.print(currentSubmenuTitle);
    }

    prevSelection = currentSelection;
}

void Menu::loop() {
    button1.loop();
    button2.loop();
}
