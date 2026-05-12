#include "menu.h"

namespace IBusko {
    const MenuItem STUDY_RUSSIAN = {
        "1 - Russian Language",
        study_russian,
        SUBJECT,
        NULL,
        0
    };

    const MenuItem STUDY_LITERATURE = {
        "2 - Literature",
        study_literature,
        SUBJECT,
        NULL,
        0
    };

    const MenuItem STUDY_MATH = {
        "3 - Math",
        study_math,
        SUBJECT,
        NULL,
        0
    };

    const MenuItem STUDY_PE = {
        "4 - Physical Education",
        study_pe,
        SUBJECT,
        NULL,
        0
    };

    const MenuItem GO_BACK_CLASS = {
        "0 - Go back to previous menu",
        go_back_class,
        SUBJECT,
        NULL,
        0
    };

    const MenuItem* subject_1_children[] = {
        &GO_BACK_CLASS,
        &STUDY_RUSSIAN,
        &STUDY_LITERATURE,
        &STUDY_MATH,
        &STUDY_PE
    };
    const int subject_1_size = sizeof(subject_1_children) / sizeof(subject_1_children[0]);

    const MenuItem CLASS_1 = {
        "1 - Study 1st grade subjects",
        NULL,
        CLASS,
        subject_1_children,
        subject_1_size
    };

    const MenuItem CLASS_2 = {
        "2 - Study 2nd grade subjects",
        NULL,
        CLASS,
        NULL,
        0
    };

    const MenuItem CLASS_3 = {
        "3 - Study 3rd grade subjects",
        NULL,
        CLASS,
        NULL,
        0
    };

    const MenuItem GO_BACK_MAIN = {
        "0 - Go back to main menu",
        go_back_main,
        CLASS,
        NULL,
        0
    };

    const MenuItem* class_children[] = {
        &GO_BACK_MAIN,
        &CLASS_1,
        &CLASS_2,
        &CLASS_3
    };
    const int class_size = sizeof(class_children) / sizeof(class_children[0]);

    const MenuItem SELECT_CLASS = {
        "1 - Choose grade to study",
        NULL,
        MAIN,
        class_children,
        class_size
    };

    const MenuItem EXIT_MENU = {
        "0 - Exit",
        exit_program,
        MAIN,
        NULL,
        0
    };

    const MenuItem* main_children[] = {
        &EXIT_MENU,
        &SELECT_CLASS
    };
    const int main_size = sizeof(main_children) / sizeof(main_children[0]);

    const MenuItem MAIN_MENU = {
        "Main Menu",
        NULL,
        MAIN,
        main_children,
        main_size
    };
}
