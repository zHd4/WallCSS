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

class Wallhack {
private:
    DWORD pid;
    HWND hWindow;
    HANDLE hProcess;

    bool flashing = false;
    short state = DISABLE_WALLHACK;

    uintptr_t wallhackAddress;

    void setState() {
        ReadProcessMemory(hProcess, (void*) wallhackAddress, &state, sizeof(state), nullptr);
    }

    void changeDrawMode(short mode, short* statePtr) {
        *statePtr = mode;
        WriteProcessMemory(hProcess, (LPVOID) wallhackAddress, &mode, sizeof(mode), nullptr);
    }

public:
    explicit Wallhack(const Config &config) {
        hWindow = FindWindowA(nullptr, config.window.c_str());
        GetWindowThreadProcessId(hWindow, &pid);

        hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
        wallhackAddress = config.offset;

        setState();
    }

    bool isAvailable() {
        try {
            isActive();

            if(hWindow != nullptr){
                return true;
            }

            return false;
        } catch (...) {
            return false;
        }
    }

    bool isActive() const {
        return state == ENABLE_WALLHACK;
    }

    bool isFlashing() const {
        return flashing;
    }

    void toggle() {
        changeDrawMode(state == ENABLE_WALLHACK ? DISABLE_WALLHACK : ENABLE_WALLHACK, &state);
    }

    void toggleFlashing() {
        flashing = !flashing;

        if(state == ENABLE_WALLHACK) {
            toggle();
        }
    }
};

#endif
