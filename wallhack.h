//
// Created by zHd4 on 31.07.2020.
//

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

const uintptr_t WH_ADDRESS = 0x243B0C9C;


class MemAccessException : public exception {
private:
    string message;

public:
    explicit MemAccessException(const string& msg);
    const char* what() const noexcept override;
};

class Wallhack {
private:
    string gameWindowName;
    DWORD pid;
    HWND hWindow;
    HANDLE hProcess;

    bool flashing = false;
    short state = DISABLE_WALLHACK;

    short getNextState();
    void setStateFromMemory();
    void changeDrawMode(short mode, short* statePtr);

public:
    Wallhack(const string& gameWindowName);

    void init();

    bool isAvailable();
    bool isActive();
    bool isFlashing();

    void toggle();
    void toggleFlashing();
};

#endif
