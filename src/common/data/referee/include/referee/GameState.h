//
// Created by rolf on 20-03-20.
//

#ifndef SOCCER_GAMESTATE_H
#define SOCCER_GAMESTATE_H
#include "GameStage.h"
#include "GameCommand.h"
#include "GameEvent.h"
#include "TeamInfo.h"

#include <core/Time.h>
#include <math/geometry/Vector2.h>
#include <protobuf/GameState.pb.h>
class RefereeState{
    public:
        explicit RefereeState(const proto::RefereeState& gameState);
        Time timeStamp;

        GameStage stage;
        std::optional<Time> stageTimeLeft;

        GameCommand command;
        std::optional<GameCommand> nextCommand;
        unsigned int commandsSinceStartup;
        Time commandTime;

        std::optional<Vector2> designatedPosition; //Position at which the ball should be placed

        TeamInfo usInfo;
        TeamInfo themInfo;

        std::vector<GameEvent> newEvents;
        std::optional<Time> currentActionTimeRemaining;
};
class GameState {
    public:
        explicit GameState(const proto::GameState& gameState);
        //If true, we play on the positive half, else we play on the negative half (default).
        bool wePlayOnPositiveHalf = false;
        Team ourColor;
        RobotID keeperID;
        std::optional<RefereeState> refState;
};

#endif //SOCCER_GAMESTATE_H
