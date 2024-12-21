#include "wallhack.h"

MemAccessException::MemAccessException(const string& msg) : message(msg) {}

const char* MemAccessException::what() const noexcept {
    return message.c_str();
}

void Wallhack::setStateFromMemory() {
    ReadProcessMemory(hProcess, (void*)WH_ADDRESS, &state, sizeof(state), nullptr);
}

short Wallhack::getNextState() {
    return state == ENABLE_WALLHACK ? DISABLE_WALLHACK : ENABLE_WALLHACK;
}

void Wallhack::changeDrawMode(short mode, short* statePtr) {
    MEMORY_BASIC_INFORMATION mbi;

    if (VirtualQueryEx(hProcess, (LPVOID)WH_ADDRESS, &mbi, sizeof(mbi)) == 0) {
        throw MemAccessException("Error: invalid address " + to_string(WH_ADDRESS) + ". Code: " + to_string(GetLastError()));
    }

    if (mbi.State != MEM_COMMIT) {
        throw MemAccessException("Error: memory is not committed");
    }

    DWORD oldProtect;

    if (VirtualProtectEx(hProcess, (LPVOID)WH_ADDRESS, sizeof(mode), PAGE_READWRITE, &oldProtect)) {
        if (!WriteProcessMemory(hProcess, (LPVOID)WH_ADDRESS, &mode, sizeof(mode), nullptr)) {
            throw MemAccessException("Error: cannot write value in memory. Code: " + to_string(GetLastError()));
        }

        *statePtr = mode;
    } else {
        throw MemAccessException("Error: cannot unprotect memory. Code: " + to_string(GetLastError()));
    }

    VirtualProtectEx(hProcess, (LPVOID)WH_ADDRESS, sizeof(mode), oldProtect, &oldProtect);
}

Wallhack::Wallhack(const string& gameWindowName) : gameWindowName(gameWindowName) {}

void Wallhack::init() {
    hWindow = FindWindowA(nullptr, gameWindowName.c_str());

    if (hWindow != nullptr) {
        GetWindowThreadProcessId(hWindow, &pid);

        hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
        setStateFromMemory();
    }
}

bool Wallhack::isAvailable() {
    return hWindow != nullptr;
}

bool Wallhack::isActive() {
    return state == ENABLE_WALLHACK;
}

bool Wallhack::isFlashing() {
    return flashing;
}

void Wallhack::toggle() {
    changeDrawMode(getNextState(), &state);
}

void Wallhack::toggleFlashing() {
    if (state == ENABLE_WALLHACK) {
        changeDrawMode(getNextState(), &state);
    }

    flashing = !flashing;
}