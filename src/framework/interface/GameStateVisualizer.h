//
// Created by rolf on 30-03-20.
//

#ifndef SOCCER_GAMESTATEVISUALIZER_H
#define SOCCER_GAMESTATEVISUALIZER_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QHBoxLayout>
#include <protobuf/GameState.pb.h>
#include <QtWidgets/QLabel>
#include <referee/TeamInfo.h>
#include <referee/GameState.h>
#include <protobuf/FrameLog.pb.h>

class GameEventsWidget;
class GameStateVisualizer : public QWidget {
        Q_OBJECT

    public:
        explicit GameStateVisualizer(QWidget * parent = nullptr);
        ~GameStateVisualizer() override;

    public slots:
        void updateFrame(const proto::FrameLog& frame);
    private:
        void updateGamestate(const RefereeState &state, Team ourColor);
        void displayLeftTeam(const TeamInfo &teamInfo, Team color);
        void displayRightTeam(const TeamInfo &teamInfo, Team color);
        GameEventsWidget * gameEventsWidget;

        QLabel * command;
        QLabel * actionTime;
        QLabel * nextCommand;
        QLabel * gameStage;

        QLabel *teamNameText;
        QLabel * leftTeamName;
        QLabel * rightTeamName;

        QLabel * scoreText;
        QLabel * leftScore;
        QLabel * rightScore;

        QLabel * timeOutText;
        QLabel * leftTimeouts;
        QLabel * rightTimeouts;

        QLabel * foulText;
        QLabel * leftFouls;
        QLabel * rightFouls;

        QLabel * redCardText;
        QLabel * leftRedCards;
        QLabel * rightRedCards;

        QLabel * yellowCardText;
        QLabel * leftYellowCards;
        QLabel * rightYellowCards;

        QLabel * goalieText;
        QLabel * leftGoalie;
        QLabel * rightGoalie;

        QVBoxLayout *totalLayout;
        QVBoxLayout *sharedInfoLayout;
        QGridLayout *teamInfoLayout;

        QTimer * updateTimer;
};

#endif //SOCCER_GAMESTATEVISUALIZER_H
