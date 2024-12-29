##MenuX
MenuX is a lightweight and flexible menu library for TFT_eSPI-based projects. It provides a simple way to create interactive menus with support for scrolling, infinite nesting, and callbacks for each menu item. Perfect for embedded projects with TFT displays.

###Features
+ Simple interface to create menus and navigate between items.
+ Supports infinite nesting of submenus.
+ Scrollable menus to handle large item lists.
+ Customizable callbacks for menu items (e.g., actions or transitions).
+ Compatible with TFT_eSPI for easy integration into your display-based projects.
###Installation
To install the library, follow these steps:
+ Download or clone the repository.
+ Copy the MenuX folder to your Arduino libraries folder.
+ In the Arduino IDE, navigate to Sketch > Include Library > Manage Libraries and search for MenuX to install the library.
###Usage
Include the library in your project:

```
#include <MenuX.h>
```

Initialize the menu with your Button2, TFT_eSPI, and menu items:
```
Button2 btn1, btn2;
TFT_eSPI tft = TFT_eSPI();
MenuItem items[] = {
    MenuItem("Item 1", []() { Serial.println("Item 1 selected"); }),
    MenuItem("Item 2", []() { Serial.println("Item 2 selected"); })
};

Menu menu(btn1, btn2, tft, items, sizeof(items) / sizeof(items[0]), &offsetY);
```

Call the menu.loop() method in the main loop to process button events and navigate through the menu:
```
void loop() {
    menu.loop();
}

Use the buttons to navigate up/down and select menu items.
```
#include <MenuX.h>

Button2 btn1, btn2;
TFT_eSPI tft = TFT_eSPI();

int offsetY = 0;

MenuItem items[] = {
    MenuItem("Start", []() { Serial.println("Starting..."); }),
    MenuItem("Settings", []() { Serial.println("Opening settings..."); }),
    MenuItem("Exit", []() { Serial.println("Exiting..."); })
};

Menu menu(btn1, btn2, tft, items, sizeof(items) / sizeof(items[0]), &offsetY);

void setup() {
    Serial.begin(115200);
    tft.begin();
    menu.begin();
}

void loop() {
    menu.loop();
}
```
License
>This library is released under the MIT License.