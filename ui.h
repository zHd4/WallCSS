//
// Created by zHd4 on 04.08.2020.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#ifndef WALLCSS_UI_H
#define WALLCSS_UI_H

#include "config.h"
#include "wallhack.h"
#include <iostream>
#include <string>
#include <utility>

#define KEY_PRESSED 0x8000

using namespace std;

class UI {
private:
    Config config;
    Wallhack wallhack;

public:
    UI(Config config, Wallhack wallhack) : wallhack(wallhack) {
        this->config = move(config);
        this->wallhack = wallhack;
    }

    void console() {
        SetConsoleTitle(config.console.c_str());

        cout << "Waiting for \"" << config.window << "\" ..." << "\n";

        while (!wallhack.isAvailable(config));

        cout << "Detected!" << "\n";
        cout << "To toggle wallhack go to game and press \"" << config.toggleKeyName << "\"" << "\n";

        while (wallhack.isAvailable(config)) {
            try {
                if(GetKeyState(config.toggleKey) & KEY_PRESSED) {
                    wallhack.isActive() ? wallhack.disable() : wallhack.enable();
                }
            } catch (const std::exception &e) {
                cout << e.what();
                break;
            }

            Sleep(100);
        }
    }
};

#endif //WALLCSS_UI_H
