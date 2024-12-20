#include "wallhack.h"
#include <iostream>
#include <string>
#include <utility>

#define KEY_PRESSED 0x8000

using namespace std;

class KeyboardKey {
private:
    string name;
    DWORD value;
public:
    KeyboardKey(const string name, const DWORD value) 
        : name(name), value(value) { }

    string getName() const {
        return name;
    }

    DWORD getValue() const {
        return value;
    }
};

const string NAME = "WallCSS";
const string GAME_WINDOW_NAME = "Counter-Strike Source";

const KeyboardKey TOGGLE_WH_KEY("Insert", VK_INSERT);
const KeyboardKey TOGGLE_FLASHING_WH_KEY("Home", VK_HOME);


Wallhack wallhack(GAME_WINDOW_NAME);

bool isKeyPressed(DWORD key) {
    return GetKeyState(key) & KEY_PRESSED;
}

void loop() {
    while (wallhack.isAvailable()) {
        try {
            if (isKeyPressed(TOGGLE_WH_KEY.getValue())) {
                wallhack.toggle();
            }

            if (isKeyPressed(TOGGLE_FLASHING_WH_KEY.getValue())) {
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
    SetConsoleTitle(NAME.c_str());

    cout << "Waiting for \"" << GAME_WINDOW_NAME << "\" ..." << "\n";

    while (!wallhack.isAvailable());

    cout << "Detected!" << "\n\n";

    cout << "To toggle wallhack go to game and press \"" << TOGGLE_WH_KEY.getName() << "\"" << "\n";
    cout << "To toggle flashing wallhack go to game and press \"" << TOGGLE_FLASHING_WH_KEY.getName() << "\"" << "\n";

    loop();

    return 0;
}
