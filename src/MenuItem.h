#ifndef MENUITEM_H
#define MENUITEM_H

class MenuItem {
public:
    const char* label;   // Текст пункта меню
    const char* description; // Описание пункта меню
    void (*action)();    // Функция действия
    MenuItem* submenu;   // Подменю, если оно есть
    int submenuItemCount; // Количество элементов в подменю

    MenuItem(
        const char* label,
        const char* description,
        void (*action)(),
        MenuItem* submenu = nullptr, 
        int submenuItemCount = 0
    ) :
        label(label),
        description(description), 
        action(action), 
        submenu(submenu), 
        submenuItemCount(submenuItemCount) {}
};

#endif