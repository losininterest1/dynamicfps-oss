// 12:47 UTC+8 Leqixn

#pragma once
#include "../Module.h"
#include <optional>

class DynamicFPS : public Module {
public:
    DynamicFPS() : Module("DynamicFPS", "Misc", "Reduces the FPS on your game while afk or on other tab") {
        addSetting(&enabled);
        addSetting(&unfocusedFPS);
        addSetting(&afkFPS);
        addSetting(&afkTimeout);
    }

    Setting enabled = Setting("Enabled", true);
    Setting unfocusedFPS = Setting("Unfocused FPS", 20.0f, 1.0f, 60.0f);
    Setting afkFPS = Setting("AFK FPS", 30.0f, 1.0f, 60.0f);
    Setting afkTimeout = Setting("AFK Timeout", 60.0f, 5.0f, 300.0f);

    void onTick() override;
    void onDisable() override;

private:
    bool isAFK() const;
    void setLimit(int fps);
    
    std::optional<int> originalLimit;
    bool isThrottled = false;
};
