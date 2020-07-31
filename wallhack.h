#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
//
// Created by zHd4 on 31.07.2020.
//
#include <cstdint>
#include <windows.h>
#include <tlhelp32.h>

#ifndef WALLCSS_WALLHACK_H
#define WALLCSS_WALLHACK_H

#define ENABLE_WALLHACK 2
#define DISABLE_WALLHACK 1

class Wallhack {
private:
    DWORD pid;
    HWND handleWindow;
    HANDLE processHandle;

    uintptr_t whPtr;

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

    void changeDrawMode(int mode) {
        WriteProcessMemory(processHandle, (LPVOID) whPtr, &mode, sizeof(mode), nullptr);
    }

public:
    Wallhack(char* window, char* clientLib, uintptr_t offset) {
        handleWindow = FindWindowA(nullptr, window);
        GetWindowThreadProcessId(handleWindow, &pid);

        processHandle = OpenProcess(PROCESS_VM_READ, FALSE, pid);
        uintptr_t clientBase = getModuleBaseAddress(pid, clientLib);
        whPtr = clientBase + offset;
    }

    bool isAvailable() {
        try {
            isActive();
            return true;
        } catch (...) {
            return false;
        }
    }

    bool isActive() {
        int drawMode = 1;
        ReadProcessMemory(processHandle, (void*) whPtr, &drawMode, sizeof(drawMode), nullptr);

        if(drawMode == 2) {
            return true;
        }

        return false;
    }

    void enable() {
        changeDrawMode(ENABLE_WALLHACK);
    }

    void disable() {
        changeDrawMode(DISABLE_WALLHACK);
    }
};

#endif //WALLCSS_WALLHACK_H

#pragma clang diagnostic pop