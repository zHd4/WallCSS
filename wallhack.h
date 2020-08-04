//
// Created by zHd4 on 31.07.2020.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

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
    HWND handleWindow;
    HANDLE processHandle;

    int state;
    bool flashing = false;

    uintptr_t whPtr;

    void init(const Config &config) {
        handleWindow = FindWindowA(nullptr, config.window.c_str());
        GetWindowThreadProcessId(handleWindow, &pid);

        processHandle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
        uintptr_t clientBase = getModuleBaseAddress(const_cast<char *>(config.clientLib.c_str()));
        whPtr = clientBase + config.offset;

        setState();
    }

    void setState() {
        ReadProcessMemory(processHandle, (void*) whPtr, &state, sizeof(state), nullptr);
    }

    void changeDrawMode(int mode, int* stateVar) {
        *stateVar = mode;
        WriteProcessMemory(processHandle, (LPVOID) whPtr, &mode, sizeof(mode), nullptr);
    }

    uintptr_t getModuleBaseAddress(char* moduleName) const {
        uintptr_t moduleBaseAddress = 0;
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);

        if (snapshot != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 moduleEntry32;
            moduleEntry32.dwSize = sizeof(MODULEENTRY32);

            if (Module32First(snapshot, &moduleEntry32)) {
                do {
                    if (strcmp(moduleEntry32.szModule, moduleName) == 0)
                    {
                        moduleBaseAddress = (uintptr_t) moduleEntry32.modBaseAddr;
                        break;
                    }
                } while (Module32Next(snapshot, &moduleEntry32));
            }

            CloseHandle(snapshot);
        }

        return moduleBaseAddress;
    }

public:
    explicit Wallhack(const Config &config) {
        init(config);
    }

    bool isAvailable(const Config &config) {
        try {
            init(config);
            isActive();

            if(handleWindow != nullptr){
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
