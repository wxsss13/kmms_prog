#ifndef MENU_H
#define MENU_H

#include <string>
#include <cstddef>
#include <cstdlib>

namespace IBusko {
    enum MenuType {
        MAIN,
        CLASS,
        SUBJECT
    };

    struct MenuItem {
        std::string text;
        void (*action)();
        MenuType type;
        const MenuItem** children;
        int children_size;
    };

    extern bool exit_current_menu;

    void show_menu(const MenuItem& menu);
    void exit_program();
    void study_russian();
    void study_literature();
    void study_math();
    void study_pe();
    void go_back_main();
    void go_back_class();

    extern const MenuItem MAIN_MENU;
}

#endif
