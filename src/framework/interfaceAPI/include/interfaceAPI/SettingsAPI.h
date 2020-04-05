//
// Created by rolf on 05-04-20.
//

#ifndef SOCCER_SETTINGSAPI_H
#define SOCCER_SETTINGSAPI_H

#include <protobuf/Settings.pb.h>

class SettingsAPI {
    public:
        static SettingsAPI * instance();
        proto::Settings getSettings();
        void setSettings(const proto::Settings& coreSettings);
    private:
        static SettingsAPI * staticInstance;

        std::mutex settingsMutex;
        proto::Settings settings;
};

#endif //SOCCER_SETTINGSAPI_H
