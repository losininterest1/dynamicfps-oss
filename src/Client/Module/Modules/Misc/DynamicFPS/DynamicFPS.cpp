#include "DynamicFPS.hpp"
#include "../../Client.h"
#include <Windows.h>

void DynamicFPS::onTick() {
    if (!enabled.boolValue) return;

    auto mc = Client::getMinecraft();
    auto gameSettings = mc->getOptions();
    
    HWND minecraftWindow = FindWindowA("LWJGL", nullptr);
    bool isFocused = (GetForegroundWindow() == minecraftWindow); 
    
    if (!isFocused) {
        gameSettings->framerateLimit = (int)unfocusedFPS.floatValue;
    } 
    else if (isAFK()) {
        gameSettings->framerateLimit = (int)afkFPS.floatValue;
    } 
    else {
        gameSettings->framerateLimit = 165; 
    }
}

bool DynamicFPS::isAFK() {
    LASTINPUTINFO lii;
    lii.cbSize = sizeof(LASTINPUTINFO);
    if (GetLastInputInfo(&lii)) {
        uint64_t idleTime = (GetTickCount64() - lii.dwTime) / 1000;
        return idleTime >= (uint64_t)afkTimeout.floatValue;
    }
    return false;
}
