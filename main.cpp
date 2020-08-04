#include "ui.cpp"
#include "config.h"
#include "wallhack.h"

int main() {
    Config config;
    Wallhack wallhack(config);

    console(config, wallhack);

    return 0;
}
