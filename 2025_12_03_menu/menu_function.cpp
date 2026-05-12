#include "menu.h"
#include <iostream>
#include <string>

namespace IBusko {
    bool exit_current_menu = false;

    void show_menu(const MenuItem& menu) {
        system("chcp 65001 > nul");
        while (true) {
            exit_current_menu = false;
            
            std::cout << "\n=== " << menu.text << " ===" << std::endl;
            for (int i = 0; i < menu.children_size; ++i) {
                std::cout << menu.children[i]->text << std::endl;
            }

            int choice;
            std::cout << "Your choice: ";
            if (!(std::cin >> choice)) { // 处理非数字输入
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                std::cout << "Wrong input! Enter number only." << std::endl;
                continue;
            }

            bool found = false;
            for (int i = 0; i < menu.children_size; ++i) {
                char num = menu.children[i]->text[0];
                if (num - '0' == choice) {
                    found = true;
                    if (menu.children[i]->action) {
                        menu.children[i]->action();
                        // 执行action后直接判断是否退出，避免递归干扰
                        if (exit_current_menu || menu.type == MAIN && choice == 0) {
                            return; // 直接退出当前show_menu函数，返回上一级
                        }
                    }
                    if (menu.children[i]->children != NULL && menu.children[i]->children_size > 0) {
                        show_menu(*menu.children[i]);
                    }
                    break;
                }
            }

            if (!found) {
                std::cout << "Wrong choice! Try again." << std::endl;
            }
        }
    }

    void exit_program() {
        std::cout << "I have finished school and know everything" << std::endl;
        std::exit(0);
    }

    void study_russian() {
        std::cout << "Studying Russian language..." << std::endl;
    }

    void study_literature() {
        std::cout << "Studying Literature..." << std::endl;
    }

    void study_math() {
        std::cout << "Studying Math..." << std::endl;
    }

    void study_pe() {
        std::cout << "Studying Physical Education..." << std::endl;
    }

    void go_back_main() {
        std::cout << "Returning to main menu..." << std::endl;
        exit_current_menu = true;
    }

    void go_back_class() {
        std::cout << "Returning to class selection menu..." << std::endl;
        exit_current_menu = true;
    }
}
