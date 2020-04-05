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
class MainTeamSettingsWidget;
class MainSettingsWidget : public QWidget {
        Q_OBJECT

    public:
        explicit MainSettingsWidget(QWidget * parent);
        ~MainSettingsWidget() override;
        [[nodiscard]] proto::Settings getSettings() const;

    private slots:
        void updateMode(int index);
    private:
        proto::Settings_usageMode mode = proto::Settings_usageMode_ONE_TEAM;
        QVBoxLayout * mainLayout;
        QGroupBox * settingsBox;
        QVBoxLayout * mainSettings;
        QHBoxLayout * checkBoxLayout;

        QComboBox * usageMode;


        QCheckBox * refereeCheckBox;
        bool listenToReferee = true;
        void setListenToReferee(bool listen);

        QCheckBox * loggingCheckBox;
        bool loggingOn = false;
        void setLoggingOn(bool listen);

        QHBoxLayout * teamSettingsLayout;
        MainTeamSettingsWidget * leftTeamWidget;
        MainTeamSettingsWidget * rightTeamWidget;

        QHBoxLayout * networkLayout;
        QLabel * networkText;
        QSpinBox * portBox;
        QLineEdit * IPText;

        static void setDisabledColor(QWidget* widget);
};

#endif //SOCCER_MAINSETTINGSWIDGET_H
