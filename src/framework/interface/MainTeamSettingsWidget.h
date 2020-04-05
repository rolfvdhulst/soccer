//
// Created by rolf on 05-04-20.
//

#ifndef SOCCER_MAINTEAMSETTINGSWIDGET_H
#define SOCCER_MAINTEAMSETTINGSWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGroupBox>
#include <protobuf/Settings.pb.h>
#include <referee/GameState.h>

class MainTeamSettingsWidget : public QWidget {
    Q_OBJECT

    public:
        explicit MainTeamSettingsWidget(bool isLeft, QWidget * parent);
        ~MainTeamSettingsWidget() override;
        [[nodiscard]] proto::TeamSettings getTeamSettings() const;
        void setReplay(bool isReplay);
    private:
        void setFromGameState(const proto::GameState& gameState);
        void visualizeFromGameState(const proto::GameState& gameState);
        QGroupBox * groupBox;
        QVBoxLayout * boxLayout;
        QVBoxLayout * mainLayout;
        QHBoxLayout * teamLayout;
        QHBoxLayout * networkLayout;

        QPushButton * colorButton;
        void toggleOurColor();
        void setColorVisual(bool isBlue);
        bool weAreBlue = true;

        QPushButton * sideButton;
        void toggleOurSide();
        void setSideVisual(bool wePlayOnPositiveHalf);
        bool wePlayOnPositiveHalf = false;

        QComboBox * keeperIDBox;
        void setKeeperID(const QString &id);
        int keeperID = 0;

        bool isInReplay = false;
        proto::TeamSettings replaySettings;
};

#endif //SOCCER_MAINTEAMSETTINGSWIDGET_H
