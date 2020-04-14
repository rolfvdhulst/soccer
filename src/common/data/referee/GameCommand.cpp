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
GameCommand::GameCommand(const proto::RefereeState::Command &command) {
    switch(command){
    case proto::RefereeState_Command_HALT: value = HALT; return;
    case proto::RefereeState_Command_STOP: value = STOP; return;
    case proto::RefereeState_Command_RUNNING: value = RUNNING; return;
    case proto::RefereeState_Command_PREPARE_KICKOFF_THEM: value = PREPARE_KICKOFF_THEM; return;
    case proto::RefereeState_Command_PREPARE_KICKOFF_US: value = PREPARE_KICKOFF_US; return;
    case proto::RefereeState_Command_PREPARE_PENALTY_THEM: value = PREPARE_PENALTY_THEM; return;
    case proto::RefereeState_Command_PREPARE_PENALTY_US: value = PREPARE_PENALTY_US; return;
    case proto::RefereeState_Command_DIRECT_FREE_THEM: value = DIRECT_FREE_THEM; return;
    case proto::RefereeState_Command_DIRECT_FREE_US: value = DIRECT_FREE_US; return;
    case proto::RefereeState_Command_INDIRECT_FREE_THEM: value = INDIRECT_FREE_THEM; return;
    case proto::RefereeState_Command_INDIRECT_FREE_US: value = INDIRECT_FREE_US; return;
    case proto::RefereeState_Command_TIMEOUT_THEM: value = TIMEOUT_THEM; return;
    case proto::RefereeState_Command_TIMEOUT_US: value = TIMEOUT_US; return;
    case proto::RefereeState_Command_BALL_PLACEMENT_THEM: value = BALL_PLACEMENT_THEM; return;
    case proto::RefereeState_Command_BALL_PLACEMENT_US: value = BALL_PLACEMENT_US; return;
    case proto::RefereeState_Command_KICKOFF_US: value = KICKOFF_US; return;
    case proto::RefereeState_Command_KICKOFF_THEM: value = KICKOFF_THEM; return;
    case proto::RefereeState_Command_PENALTY_US: value = PENALTY_US; return;
    case proto::RefereeState_Command_PENALTY_THEM: value = PENALTY_THEM; return;
    }
}
std::string GameCommand::toString() const {
    switch(value){
    case HALT:return "HALT";
    case STOP:return "STOP";
    case RUNNING:return "RUNNING";
    case PREPARE_KICKOFF_THEM:return "PREPARE_KICKOFF_THEM";
    case PREPARE_KICKOFF_US:return "PREPARE_KICKOFF_US";
    case PREPARE_PENALTY_THEM:return "PREPARE_PENALTY_THEM";
    case PREPARE_PENALTY_US:return "PREPARE_PENALTY_US";
    case DIRECT_FREE_THEM:return "DIRECT_FREE_THEM";
    case DIRECT_FREE_US:return "DIRECT_FREE_US";
    case INDIRECT_FREE_THEM:return "INDIRECT_FREE_THEM";
    case INDIRECT_FREE_US:return "INDIRECT_FREE_US";
    case TIMEOUT_THEM:return "TIMEOUT_THEM";
    case TIMEOUT_US:return "TIMEOUT_US";
    case BALL_PLACEMENT_THEM:return "BALL_PLACEMENT_THEM";
    case BALL_PLACEMENT_US:return "BALL_PLACEMENT_US";
    case KICKOFF_US:return "KICKOFF_US";
    case KICKOFF_THEM:return "KICKOFF_THEM";
    case PENALTY_US:return "PENALTY_US";
    case PENALTY_THEM:return "PENALTY_THEM";
    }
}
