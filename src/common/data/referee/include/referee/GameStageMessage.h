//
// Created by rolf on 20-03-20.
//

#ifndef SOCCER_GAMESTAGEMESSAGE_H
#define SOCCER_GAMESTAGEMESSAGE_H
#include "GameStage.h"
#include <core/Time.h>
#include <geometry/Vector2.h>
class GameStageMessage {
    private:
        Time lastPacketTime;
        GameStage stage;
        Time stageTimeLeft;
        //RefCommand command;
        unsigned int commandsSinceStartup;
        Vector2 designatedPosition; //Position at which the ball should be placed
        //If true, the blue team plays on the positiveHalf, else the blue team plays on the negative half.
        bool blueTeamOnPositiveHalf = false;
        Time lastCommandTime;
        //TeamInfo blueInfo
        //TeamInfo yellowInfo
        //GameEvent lastEvent
        //RefCommand nextCommand;
};

#endif //SOCCER_GAMESTAGEMESSAGE_H
