//
// Created by rolf on 20-03-20.
//

#include "GameState.h"
GameState::GameState(const proto::GameState &gameState) :
        wePlayOnPositiveHalf{gameState.settings().weplayonpositivehalf()},
        keeperID{RobotID(gameState.settings().keeperid())}
{
    if(gameState.settings().weareblue()){
        ourColor = Team(Team::BLUE);
    }else{
        ourColor = Team(Team::YELLOW);
    }
    if(gameState.has_referee()){
        refState = RefereeState(gameState.referee());
    }
}
RefereeState::RefereeState(const proto::RefereeState &gameState) :
        timeStamp{Time(gameState.timestamp())},
        stage{GameStage(gameState.stage())},
        command{GameCommand(gameState.command())},
        commandsSinceStartup{gameState.command_counter()},
        commandTime{Time(gameState.command_timestamp())},
        usInfo{TeamInfo(gameState.us())},
        themInfo{TeamInfo(gameState.them())}
{
    if (gameState.has_stage_time_left()){
        stageTimeLeft = Time(gameState.stage_time_left());
    }
    if (gameState.has_designated_position()){
        designatedPosition = Vector2(gameState.designated_position());
    }
    if (gameState.has_nextcommand()){
        nextCommand = GameCommand(gameState.nextcommand());
    }
    for (const auto & event : gameState.game_events()){
        newEvents.emplace_back(event);
    }
    if(gameState.has_current_action_time_remaining()){
        currentActionTimeRemaining = Time(gameState.current_action_time_remaining());
    }
}