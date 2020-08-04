//
// Created by zHd4 on 31.07.2020.
//

#include <string>
#include <cstdint>
#include <Windows.h>

#ifndef WALLCSS_CONFIG_H
#define WALLCSS_CONFIG_H

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wwritable-strings"

using namespace std;

class Config {
public:
    string console = "WallCSS";
    string window = "Counter-Strike Source";
    string clientLib = "client.dll";

    // Absolute offset is 0x243B0C9C
    uintptr_t offset = 0x3B0C9C;

    DWORD toggleKey = VK_INSERT;
    string toggleKeyName = "INSERT";
};

#endif
