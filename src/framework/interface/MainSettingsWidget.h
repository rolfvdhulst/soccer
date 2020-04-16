//
// Created by rolf on 05-04-20.
//

#ifndef SOCCER_MAINSETTINGSWIDGET_H
#define SOCCER_MAINSETTINGSWIDGET_H

#include <QtWidgets/QWidget>
#include <QVBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGroupBox>
#include <protobuf/Settings.pb.h>
#include <QtWidgets/QComboBox>
#include <protobuf/GameState.pb.h>
#include "ReplayWidget.h"

class MainTeamSettingsWidget;
class MainSettingsWidget : public QWidget {
        Q_OBJECT

    public slots:
        //This function is only for visualization during replay!
        void visualizeFrame(const proto::FrameLog &frame);
        void saveBacklog();
    public:
        explicit MainSettingsWidget(QWidget * parent);
        ~MainSettingsWidget() override;
        [[nodiscard]] proto::Settings getSettings();
        void updateFrame(const proto::FrameLog &frame);
        ReplayWidget * getReplayWidget();
    private slots:
        void updateMode(int index);
    private:
        proto::Settings_usageMode mode = proto::Settings_usageMode_ONE_TEAM;
        QVBoxLayout * mainLayout;
        QGroupBox * settingsBox;
        QVBoxLayout * mainSettings;
        QHBoxLayout * checkBoxLayout;

        ReplayWidget * replayWidget;

        QComboBox * usageMode;


        QCheckBox * refereeCheckBox;
        bool listenToReferee = true;
        void setListenToReferee(bool listen);

        QCheckBox * loggingCheckBox;
        bool loggingOn = false;
        void setLoggingOn(bool listen);

        QCheckBox * replayCheckBox;
        bool playReplay = false;
        void setPlayReplay(bool play);

        QHBoxLayout * teamSettingsLayout;
        MainTeamSettingsWidget * leftTeamWidget;
        MainTeamSettingsWidget * rightTeamWidget;

        QHBoxLayout * networkLayout;
        QLabel * networkText;
        QSpinBox * portBox;
        QLineEdit * IPText;

        int messageCounter = 0;
        bool saveBacklogNextTick = false;

        static void setDisabledColor(QWidget* widget);
        void showUsageMode(const proto::Settings_usageMode &newMode) const;
};

#endif //SOCCER_MAINSETTINGSWIDGET_H
