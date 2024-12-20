#include "wallhack.h"
#include <iostream>
#include <string>
#include <utility>

#define KEY_PRESSED 0x8000

#define MAIN_LOOP_DELAY 70
#define FLASHING_WH_TOGGLING_DELAY 30

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
                Sleep(FLASHING_WH_TOGGLING_DELAY);
            }
        } catch (const MemAccessException& e) {
            cout << e.what();
        } catch (const exception& e) {
            cout << e.what();
            break;
        }

        Sleep(MAIN_LOOP_DELAY);
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
