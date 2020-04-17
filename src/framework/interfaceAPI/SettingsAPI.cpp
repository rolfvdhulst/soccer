//
// Created by rolf on 05-04-20.
//

#include "SettingsAPI.h"
SettingsAPI* SettingsAPI::staticInstance = nullptr;

SettingsAPI* SettingsAPI::instance() {
    if (!staticInstance){
        staticInstance = new SettingsAPI();
    }
    return staticInstance;
}
proto::Settings SettingsAPI::getSettings() {
    std::lock_guard<std::mutex> lock(settingsMutex);
    return settings;
}
void SettingsAPI::setSettings(const proto::Settings &coreSettings) {
    std::lock_guard<std::mutex> lock(settingsMutex);
    hasReceivedSettings = true;
    settings = coreSettings;
}
bool SettingsAPI::receivedFirstSettings() {
    return hasReceivedSettings;
}
