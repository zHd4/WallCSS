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

class MemAccessException : public std::exception {
private:
    string message;

public:
    explicit MemAccessException(const string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

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
        MEMORY_BASIC_INFORMATION mbi;

        if (VirtualQueryEx(hProcess, (LPVOID)wallhackAddress, &mbi, sizeof(mbi)) == 0) {
            throw MemAccessException("Error: invalid address " + to_string(wallhackAddress) + ". Code: " + to_string(GetLastError()));
        }

        if (mbi.State != MEM_COMMIT) {
            throw MemAccessException("Error: memory is not committed");
        }

        DWORD oldProtect;

        if (VirtualProtectEx(hProcess, (LPVOID)wallhackAddress, sizeof(mode), PAGE_READWRITE, &oldProtect)) {
            if (!WriteProcessMemory(hProcess, (LPVOID)wallhackAddress, &mode, sizeof(mode), nullptr)) {
                throw MemAccessException("Error: cannot write value in memory. Code: " + to_string(GetLastError()));
            }

            *statePtr = mode;
        } else {
            cerr << "Error: Cannot unprotect memory. Code: " << GetLastError() << endl;
        }

        VirtualProtectEx(hProcess, (LPVOID)wallhackAddress, sizeof(mode), oldProtect, &oldProtect);
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
