//
// Created by zHd4 on 04.08.2020.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include <string>
#include "config.h"
#include "wallhack.h"
#include <iostream>

#define KEY_PRESSED 0x8000

using namespace std;

void console(Config config, Wallhack wallhack) {
    SetConsoleTitle(config.console);

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