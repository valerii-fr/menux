#ifndef MENUITEM_H
#define MENUITEM_H

class MenuItem {
public:
    const char* label;
    const char* description;
    void (*action)();
    MenuItem* submenu;
    int submenuItemCount;

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