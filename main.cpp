#include "ui.h"
#include "config.h"
#include "wallhack.h"

int main() {
    Config config;
    Wallhack wallhack(config);
    UI ui(config, wallhack);

    ui.processUI();

    return 0;
}
