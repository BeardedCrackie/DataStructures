﻿#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <libds/heap_monitor.h>

//using namespace std;

typedef std::function<void()> callback_function;

/*
 * abstract class used as template for menu items
 */
class MenuItem {
protected:
    std::string name;
public:
    virtual ~MenuItem() = default;
    MenuItem(std::string name) {
        this->name = name;
    };
    virtual void apply() = 0;
    std::string getName() {
        return this->name;
    };
};

/*
 * class used for menu items that points to function
 */
class MenuActionItem : public MenuItem {
private:
    callback_function action_function;
    callback_function prepare_function;
public:
    MenuActionItem(std::string name, callback_function action_function) : MenuItem(name) {
        this->action_function = action_function;
        this->prepare_function = nullptr;
    };
    MenuActionItem(std::string name, callback_function action_function, callback_function prepare_function) : MenuItem(name) {
        this->action_function = action_function;
        this->prepare_function = prepare_function;
    };
    virtual void apply() override {
        if (prepare_function != nullptr) {
            prepare_function();
        }
        action_function();
    };
};

/*
 * class represents menu item in console application
 */
class CliMenu : public MenuItem {
private:
    std::vector<MenuItem*> menu_list;
public:
    CliMenu(std::string menu_name) : MenuItem(menu_name) {
        this->menu_list = std::vector<MenuItem*>();
    };
    ~CliMenu();
    void PrintMenu();
    void AddItem(MenuItem* item);
    void SelectItem();
    void RunItem(MenuItem* item);
    void apply() {
        this->SelectItem();
    };
};

CliMenu::~CliMenu() {
    std::cout << "deleting: " << getName() << std::endl;

    for (auto menuItem : menu_list) {
        //std::cout << "deleting: " << menuItem->getName() << std::endl;
        delete menuItem;
    }
    menu_list.clear();
}

void CliMenu::PrintMenu() {
    printf("\n=== %s ===\n", this->getName().c_str());
    for (int i = 0; i < this->menu_list.size(); i++) {
        printf("> %d: %s\n", i + 1, this->menu_list[i]->getName().c_str());
    }
    printf("> %s: %s\n", "e", "exit");
    printf("\n");
}

void CliMenu::AddItem(MenuItem* item) {
    this->menu_list.push_back(item);
}

void CliMenu::SelectItem() {
    bool selected = false;
    char choice;
    while (!selected) {
        this->PrintMenu();
        std::cout << "select item from menu: " << std::endl;
        std::cin >> choice;
        int c = (int)choice - (int)'0' - 1;
        if (c >= 0 && c < (int)this->menu_list.size()) {
            this->menu_list[c]->apply();
        }
        else if (choice == 'e') {
            return;
        }
        //todo quit option else if (choice == 'q') {
        //    return;
        //
        else {
            std::cout << "Wrong choice" << std::endl;
        }
    }
}

void CliMenu::RunItem(MenuItem* item) {
    item->apply();
}