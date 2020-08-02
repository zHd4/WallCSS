#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

//
// Created by yevhe on 01.08.2020.
//

#ifndef WALLCSS_UI_H
#define WALLCSS_UI_H

#include "config.h"
#include "wallhack.h"
#include <iostream>
#include <string>

using namespace std;

class UI {
private:
    Config config;
    Wallhack wallhack;

    char *exitCommand = "q";

    bool ifExit() {
        string command;
        getline(cin, command);

        if (command == exitCommand) {
            return true;
        }

        return false;
    }

    bool activationMessage() {
        cout << "To activate wallhack press enter, to exit press \"" << exitCommand << "\" ";
        return !ifExit();
    }

    bool deactivationMessage() {
        cout << "To deactivate wallhack press enter, to exit press \"" << exitCommand << "\" ";
        return !ifExit();
    }

public:
    UI(Config config, Wallhack wallhack) : wallhack(wallhack) {
        this->config = config;
        this->wallhack = wallhack;
    }

    void printExit() {
        cout << "Exiting\n";
    }

    void console() {
        cout << "Waiting for \"" << config.window << "\" ...\n";

        while (!wallhack.isAvailable(config));

        cout << "Detected!\n";

        while (wallhack.isAvailable(config)) {
            try {
                if (wallhack.isActive()) {
                    if(!deactivationMessage()) {
                        return;
                    }

                    wallhack.disable();
                } else {
                    if(!activationMessage()) {
                        return;
                    }

                    wallhack.enable();
                }
            } catch (const std::exception &e) {
                cout << e.what();
                break;
            }
        }
    }
};

#endif //WALLCSS_UI_H
