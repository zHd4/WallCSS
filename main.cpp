#include "config.h"
#include "wallhack.h"
#include <iostream>
#include <string>
#include <utility>

#define KEY_PRESSED 0x8000

using namespace std;

Config config;
Wallhack wallhack(config);

bool isKeyPressed(DWORD key) {
    return GetKeyState(key) & KEY_PRESSED;
}

void loop() {
    while (wallhack.isAvailable()) {
        try {
            if (isKeyPressed(config.toggleWallhackKey)) {
                wallhack.toggle();
            }

            if (isKeyPressed(config.toggleFlashingKey)) {
                wallhack.toggleFlashing();
            }

            if (wallhack.isFlashing()) {
                wallhack.toggle();
                Sleep(30);
            }
        }
        catch (const std::exception& e) {
            cout << e.what();
            break;
        }

        Sleep(70);
    }
}

int main() {
    SetConsoleTitle(config.console.c_str());

    cout << "Waiting for \"" << config.window << "\" ..." << "\n";

    while (!wallhack.isAvailable());

    cout << "Detected!" << "\n\n";

    cout << "To toggle wallhack go to game and press \"" << config.toggleWallhackKeyName << "\"" << "\n";
    cout << "To toggle flashing wallhack go to game and press \"" << config.toggleFlashingKeyName << "\"" << "\n";

    loop();

    return 0;
}
