#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
//
// Created by yevhe on 31.07.2020.
//
#include "config.h"
#include <cstdint>
#include <windows.h>
#include <tlhelp32.h>

#ifndef WALLCSS_WALLHACK_H
#define WALLCSS_WALLHACK_H

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
                        moduleBaseAddress = (uintptr_t)moduleEntry32.modBaseAddr;
                        break;
                    }
                } while (Module32Next(hSnapshot, &moduleEntry32));
            }

            CloseHandle(hSnapshot);
        }

        return moduleBaseAddress;
    }

public:
    void init(char* window, char* clientLib, uintptr_t offset) {
        handleWindow = FindWindowA(nullptr, window);
        GetWindowThreadProcessId(handleWindow, &pid);

        processHandle = OpenProcess(PROCESS_VM_READ, FALSE, pid);
        uintptr_t clientBase = getModuleBaseAddress(pid, clientLib);
        whPtr = clientBase + offset;
    }


};

#endif //WALLCSS_WALLHACK_H

#pragma clang diagnostic pop