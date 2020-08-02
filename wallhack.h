#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

//
// Created by zHd4 on 31.07.2020.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"

#include "config.h"
#include <cstdint>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

#ifndef WALLCSS_WALLHACK_H
#define WALLCSS_WALLHACK_H

#define ENABLE_WALLHACK 2
#define DISABLE_WALLHACK 1

class Wallhack {
private:
    DWORD pid;
    HWND handleWindow;
    HANDLE processHandle;

    int state;
    uintptr_t whPtr;

    void init(Config config) {
        handleWindow = FindWindowA(nullptr, config.window);
        GetWindowThreadProcessId(handleWindow, &pid);

        processHandle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
        uintptr_t clientBase = getModuleBaseAddress(pid, config.clientLib);
        whPtr = clientBase + config.offset;

        setState();
    }

    void setState() {
        ReadProcessMemory(processHandle, (void*) whPtr, &state, sizeof(state), nullptr);
    }

    void changeDrawMode(int mode) {
        WriteProcessMemory(processHandle, (LPVOID) whPtr, &mode, sizeof(mode), nullptr);
    }

    uintptr_t getModuleBaseAddress(DWORD dwProcID, char* szModuleName)
    {
        uintptr_t moduleBaseAddress = 0;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcID);

        if (hSnapshot != INVALID_HANDLE_VALUE)
        {
            MODULEENTRY32 moduleEntry32;
            moduleEntry32.dwSize = sizeof(MODULEENTRY32);

            if (Module32First(hSnapshot, &moduleEntry32))
            {
                do
                {
                    if (strcmp(moduleEntry32.szModule, szModuleName) == 0)
                    {
                        moduleBaseAddress = (uintptr_t) moduleEntry32.modBaseAddr;
                        break;
                    }
                } while (Module32Next(hSnapshot, &moduleEntry32));
            }

            CloseHandle(hSnapshot);
        }

        return moduleBaseAddress;
    }

public:
    explicit Wallhack(Config config) {
        init(config);
    }

    bool isAvailable(Config config) {
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
        if(state == 2) {
            return true;
        }

        return false;
    }

    void enable() {
        state = ENABLE_WALLHACK;
        changeDrawMode(state);
    }

    void disable() {
        state = DISABLE_WALLHACK;
        changeDrawMode(state);
    }
};

#endif
