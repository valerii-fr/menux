#ifndef MENUX_H
#define MENUX_H

#include <Arduino.h>
#include <Button2.h>
#include <TFT_eSPI.h>
#include "MenuItem.h"
#include "flow.h"
#include <stack>
#include <memory>

#define DEBOUNCE_TIME                   50
#define LONG_CLICK_TIME                 300

#define BUTTON_1                        35
#define BUTTON_2                        0

#ifndef MENU_FONT_SIZE
    #define MENU_FONT_SIZE              1
#endif

#ifndef MENU_FONT
    #define MENU_FONT                   1
#endif

#ifndef ROW_HEIGHT
    #define ROW_HEIGHT                  12
#endif

#ifndef ROW_SHIFT
    #define ROW_SHIFT                   3
#endif 

#ifndef MENU_BG_COLOR
    #define MENU_BG_COLOR               TFT_BLACK
#endif

#ifndef MENU_DIVIDER_COLOR
    #define MENU_DIVIDER_COLOR          TFT_DARKGREY
#endif

#ifndef MENU_SEL_COLOR
    #define MENU_SEL_COLOR              TFT_BLUE
#endif

#ifndef MENU_TEXT_COLOR
    #define MENU_TEXT_COLOR             TFT_WHITE
#endif

#ifndef MENU_TEXT_COLOR_VARIANT
    #define MENU_TEXT_COLOR_VARIANT     TFT_GOLD
#endif

class Menu {
public:
    Menu(
        Button2 &btn1,
        Button2 &btn2,
        TFT_eSPI &tft,
        MenuItem items[],
        int itemCount,
        int *offsetY
    );

    struct MenuContext {
        MenuItem* menuItems;                // Menu items pointers
        int menuItemCount;                  // Menu items count
        String currentItemLabel;
        String currentItemAction;
        String currentSectionLabel;
    };

    bool isMenuActive = true; 

    void begin();
    void loop();
    void drawMenu();
    void back();
    void transitionToInteractor();
    void returnToMenu();
    void clearMenuField();
    Flow<MenuContext>& getMenuFlow();

private:
    Button2 &button1;
    Button2 &button2;
    TFT_eSPI &tft;
    MenuItem* items[10];
    MenuItem *currentMenuItems;
    String currentSubmenuTitle;
    std::stack<MenuContext> menuStack;
    MenuContext menuContext = MenuContext{*items, itemCount, "", ""};
    Flow<MenuContext> menuFlow;
    int scrollOffset = 0;
    int currentMenuItemCount;
    int currentItemIndex = 0;
    int itemCount;
    int currentSelection = 0;
    int prevSelection;
    int *offsetY = 0;
    
    void navigateUp();
    void navigateDown();
    void handleButton1Press();
    void handleButton2Press();
    void handleButtonLongPress();
    void select();
    void disableButtons();
    void enableButtons();
    void publishMenuContext();
    void updateMenuContext();
};

#endif
