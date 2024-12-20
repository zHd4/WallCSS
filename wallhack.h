//
// Created by zHd4 on 31.07.2020.
//

#include "config.h"
#include <string>
#include <cstdint>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

#ifndef WALLCSS_WALLHACK_H
#define WALLCSS_WALLHACK_H

#define ENABLE_WALLHACK 2
#define DISABLE_WALLHACK 1

using namespace std;

class MemAccessException : public exception {
private:
    string message;

public:
    explicit MemAccessException(const string& msg);
    const char* what() const noexcept override;
};

class Wallhack {
private:
    DWORD pid;
    HWND hWindow;
    HANDLE hProcess;

    bool flashing = false;
    short state = DISABLE_WALLHACK;

    uintptr_t wallhackAddress;

    void setStateFromMemory();
    void changeDrawMode(short mode, short* statePtr);

public:
    explicit Wallhack(const Config& config);

    bool isAvailable();
    bool isActive();
    bool isFlashing();

    void toggle();
    void toggleFlashing();
};

#endif
