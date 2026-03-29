// 12:44 UTC+8 Leqixn

#include "DynamicFPS.hpp"
#include "../../../SDK/Minecraft.h"
#include <Windows.h>

void DynamicFPS::onTick() {
    if (!enabled.boolValue) return;

    auto instance = Minecraft::getClientInstance();
    if (!instance || !instance->getOptions()) return;

    auto options = instance->getOptions();

    HWND foreground = GetForegroundWindow();
    char className[256];
    GetClassNameA(foreground, className, sizeof(className));
    
    bool isTabbedOut = (strcmp(className, "ApplicationFrameWindow") != 0 && 
                        strcmp(className, "Windows.UI.Core.CoreWindow") != 0);

    if (isTabbedOut || isAFK()) {
        if (!isThrottled) {
            originalLimit = options->framerateLimit;
            isThrottled = true;
        }
        options->framerateLimit = isTabbedOut ? (int)unfocusedFPS.floatValue : (int)afkFPS.floatValue;
    } 
    else if (isThrottled) {
        if (originalLimit.has_value()) {
            options->framerateLimit = originalLimit.value();
        }
        isThrottled = false;
    }
}

bool DynamicFPS::isAFK() const {
    LASTINPUTINFO lii = { sizeof(LASTINPUTINFO) };
    if (GetLastInputInfo(&lii)) {
        uint64_t idleTime = (GetTickCount64() - lii.dwTime) / 1000;
        return idleTime >= (uint64_t)afkTimeout.floatValue;
    }
    return false;
}

void DynamicFPS::onDisable() {
    if (isThrottled && originalLimit.has_value()) {
        auto instance = Minecraft::getClientInstance();
        if (instance && instance->getOptions()) {
            instance->getOptions()->framerateLimit = originalLimit.value();
        }
    }
    isThrottled = false;
}
