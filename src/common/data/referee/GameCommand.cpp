//
// Created by rolf on 27-03-20.
//

#include "GameCommand.h"
constexpr bool GameCommand::operator==(GameCommand other) const {
    return value == other.value;
}
constexpr bool GameCommand::operator!=(GameCommand other) const {
    return value != other.value;
}
GameCommand::operator Command() const {
    return value;
}
constexpr GameCommand::GameCommand(GameCommand::Command command) :value(command){ }
GameCommand::GameCommand() : value{HALT}{ }
GameCommand::GameCommand(const proto::GameState::Command &command) {
    switch(command){
    case proto::GameState_Command_HALT: value = HALT; return;
    case proto::GameState_Command_STOP: value = STOP; return;
    case proto::GameState_Command_RUNNING: value = RUNNING; return;
    case proto::GameState_Command_PREPARE_KICKOFF_THEM: value = PREPARE_KICKOFF_THEM; return;
    case proto::GameState_Command_PREPARE_KICKOFF_US: value = PREPARE_KICKOFF_US; return;
    case proto::GameState_Command_PREPARE_PENALTY_THEM: value = PREPARE_PENALTY_THEM; return;
    case proto::GameState_Command_PREPARE_PENALTY_US: value = PREPARE_PENALTY_US; return;
    case proto::GameState_Command_DIRECT_FREE_THEM: value = DIRECT_FREE_THEM; return;
    case proto::GameState_Command_DIRECT_FREE_US: value = DIRECT_FREE_US; return;
    case proto::GameState_Command_INDIRECT_FREE_THEM: value = INDIRECT_FREE_THEM; return;
    case proto::GameState_Command_INDIRECT_FREE_US: value = INDIRECT_FREE_US; return;
    case proto::GameState_Command_TIMEOUT_THEM: value = TIMEOUT_THEM; return;
    case proto::GameState_Command_TIMEOUT_US: value = TIMEOUT_US; return;
    case proto::GameState_Command_BALL_PLACEMENT_THEM: value = BALL_PLACEMENT_THEM; return;
    case proto::GameState_Command_BALL_PLACEMENT_US: value = BALL_PLACEMENT_US; return;
    case proto::GameState_Command_KICKOFF_US: value = KICKOFF_US; return;
    case proto::GameState_Command_KICKOFF_THEM: value = KICKOFF_THEM; return;
    case proto::GameState_Command_PENALTY_US: value = PENALTY_US; return;
    case proto::GameState_Command_PENALTY_THEM: value = PENALTY_THEM; return;
    }
}
