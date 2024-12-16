//
// Created by zHd4 on 31.07.2020.
//

#include <string>
#include <cstdint>
#include <Windows.h>

#ifndef WALLCSS_CONFIG_H
#define WALLCSS_CONFIG_H

using namespace std;

class Config {
public:
    string console = "WallCSS";
    string window = "Counter-Strike Source";
    string clientLib = "client.dll";

    // Absolute offset is 0x243B0C9C
    uintptr_t offset = 0x3B0C9C;

    DWORD toggleWallhackKey = VK_INSERT;
    DWORD toggleFlashingKey = VK_HOME;

    string toggleWallhackKeyName = "INSERT";
    string toggleFlashingKeyName = "HOME";
};

#endif
