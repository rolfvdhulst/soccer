//
// Created by rolf on 20-03-20.
//

#ifndef SOCCER_GAMESTATE_H
#define SOCCER_GAMESTATE_H
#include "GameStage.h"
#include <core/Time.h>
#include <geometry/Vector2.h>
class GameState {
    private:
        Time lastPacketTime;
        GameStage stage;
        Time stageTimeLeft;
        //RefCommand command;
        //std::optional<RefCommand> predictedNextCommand;
        unsigned int commandsSinceStartup;
        Vector2 designatedPosition; //Position at which the ball should be placed
        //If true, the blue team plays on the positiveHalf, else the blue team plays on the negative half.
        bool blueTeamOnPositiveHalf = false;
        Time lastCommandTime;
        //TeamInfo usInfo
        //TeamInfo themInfo
        //GameEvent lastEvent
};

#endif //SOCCER_GAMESTATE_H
