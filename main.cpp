#include "config.h"
#include "wallhack.h"
#include <iostream>

char* exitCommand = "q";

void print(char* text) {
    std::cout << text;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantConditionsOC"

char* input() {
    char* result = nullptr;
    std::cin >> result;

    return result;
}

bool exitOrContinue() {
    if(input() != exitCommand) {
        return true;
    }

    return false;
}

void printExit() {
    print("Exiting\n");
}

#pragma clang diagnostic pop

int main() {
    Config config;
    Wallhack wallhack(config.window, config.clientLib, config.offset);

    print("Waiting for \"");
    print(config.window);
    print("\"\n...");

    while (!wallhack.isAvailable());

    print("Detected!\n");

    while (wallhack.isAvailable()) {
        try {
            if(wallhack.isActive()) {
                print("To activate wallhack press enter, to exit press \"");
                print(exitCommand);
                print("\"> ");

                if(!exitOrContinue()) {
                    printExit();
                    return 0;
                }

                wallhack.enable();
            } else {
                print("To deactivate wallhack press enter, to exit press \"");
                print(exitCommand);
                print("\"> ");

                if(!exitOrContinue()) {
                    printExit();
                    return 0;
                }

                wallhack.disable();
            }
        } catch (const std::exception& e) {
            print((char*)e.what());
            break;
        }
    }

    return 0;
}
