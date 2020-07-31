//
// Created by zHd4 on 31.07.2020.
//

#include <cstdint>

#ifndef WALLCSS_CONFIG_H
#define WALLCSS_CONFIG_H

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wwritable-strings"

class Config {
public:
    // Absolute offset is 0x243B0C9C
    char* window = "Counter-Strike Source";
    char* clientLib = "client.dll";

    uintptr_t offset = 0x3B0C9C;
};

#endif
