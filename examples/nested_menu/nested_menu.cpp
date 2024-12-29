#include <Arduino.h>
#include "menu.h"

#define BUTTON_1            35
#define BUTTON_2            0

// basic TFT init for TTGO T-Display
TFT_eSPI tft = TFT_eSPI(135, 240);

// menu buttons init
Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);

//  ITEM NAME       ITEM ACTION     CALLBACK
MenuItem bleConnectItems[] = {
    {"Option 1",    "Option 1",     [](){Serial.println("Option 1 was selected!");}},
    {"Option 2",    "Option 2",     [](){Serial.println("Option 2 was selected!");}},
    {"Option 3",    "Option 3",     [](){Serial.println("Option 3 was selected!");}},
    {"Option 4",    "Option 4",     [](){Serial.println("Option 4 was selected!");}},
    {"Option 5",    "Option 5",     [](){Serial.println("Option 5 was selected!");}},
    {"Option 6",    "Option 6",     [](){Serial.println("Option 6 was selected!");}},
    {"Option 7",    "Option 7",     [](){Serial.println("Option 7 was selected!");}},
    {"Option 8",    "Option 8",     [](){Serial.println("Option 8 was selected!");}},
    {"Option 9",    "Option 9",     [](){Serial.println("Option 9 was selected!");}},
    {"Option 10",   "Option 10",    [](){Serial.println("Option 10 was selected!");}},
    {"Option 11",   "Option 11",    [](){Serial.println("Option 11 was selected!");}},
    {"Option 12",   "Option 12",    [](){Serial.println("Option 12 was selected!");}},
    {"Option 13",   "Option 13",    [](){Serial.println("Option 13 was selected!");}},
};
int bleConnectItemsCount = sizeof(bleConnectItems) / sizeof(bleConnectItems[0]);

MenuItem wiFiItems[] = {
    {"Scan",        "Scan",         nullptr},
    {"Create",      "Create",       nullptr}
};
int wiFiItemsCount = sizeof(wiFiItems) / sizeof(wiFiItems[0]);

//  ITEM NAME       ITEM ACTION     CALLBACK    SUB-MENU            SUB-MENU ITEMS COUNT
MenuItem bleItems[] = {
    {"Connect",     "View",         nullptr,    bleConnectItems,    bleConnectItemsCount },
    {"Scan",        "Scan",         nullptr},
};
int bleItemsCount = sizeof(bleItems) / sizeof(bleItems[0]);

MenuItem wirelessItems[] = {
    {"BLE",         "View",         nullptr,    bleItems,           bleItemsCount },
    {"Wi-Fi",       "View",         nullptr,    wiFiItems,          wiFiItemsCount },
};
int wirelessItemsCount = sizeof(wirelessItems) / sizeof(wirelessItems[0]);

MenuItem menuItems[] = {
    {"Wireless",    "View",         nullptr,    wirelessItems,      wirelessItemsCount},
    {"Sleep",       "Turn off",     nullptr},
};

int menuItemsCount = sizeof(menuItems) / sizeof(menuItems[0]);

// menu top ofset (e.g. for status bar)
int oy = 0;
Menu menu(btn1, btn2, tft, menuItems, menuItemsCount, &oy);

void setup() {
    Serial.begin(115200);
    // basic TFT init for TTGO T-Display
    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GOLD);
    tft.setCursor(0, 0);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(1);
    tft.setSwapBytes(true);

    menu.begin();

    // set up callback for menu context changes
    menu.getMenuFlow().subscribe([](const Menu::MenuContext& context){
        Serial.print("State changed: Label=");
        Serial.print(context.currentItemLabel);
        Serial.print(", Action=");
        Serial.println(context.currentItemAction);
    });
}

void loop() {
    // loop for buttons, you can set it for each button separately like this
    // btn1.loop();
    // btn2.loop();
    menu.loop();
}
