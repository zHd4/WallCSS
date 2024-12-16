//
// Created by zHd4 on 04.08.2020.
//

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

    bool isKeyPressed(DWORD key) {
        return GetKeyState(key) & KEY_PRESSED;
    }

    void main() {
        while (wallhack.isAvailable(config)) {
            try {
                if(isKeyPressed(config.toggleWallhackKey)) {
                    wallhack.toggle();
                }

                if(isKeyPressed(config.toggleFlashingKey)) {
                    wallhack.toggleFlashing();
                }

                if(wallhack.isFlashing()) {
                    wallhack.toggle();
                    Sleep(30);
                }
            } catch (const std::exception &e) {
                cout << e.what();
                break;
            }

            Sleep(70);
        }
    }

public:
    UI(Config config, Wallhack wallhack) : wallhack(wallhack) {
        this->config = move(config);
        this->wallhack = wallhack;
    }

    void console() {
        SetConsoleTitle(config.console.c_str());

        cout << "Waiting for \"" << config.window << "\" ..." << "\n";

        while (!wallhack.isAvailable(config));

        cout << "Detected!" << "\n\n";

        cout << "To toggle wallhack go to game and press \"" << config.toggleWallhackKeyName << "\"" << "\n";
        cout << "To toggle flashing wallhack go to game and press \"" << config.toggleFlashingKeyName << "\"" << "\n";

        main();
    }
};

#endif //WALLCSS_UI_H
