//
// Created by rolf on 30-03-20.
//
#include "CommandSwitch.h"
typedef std::pair<proto::Referee_Command,proto::Referee_Command> refPair;
std::map<refPair,proto::RefereeState_Command> commandSwitch{
        {refPair(proto::Referee_Command_PREPARE_KICKOFF_BLUE,proto::Referee_Command_NORMAL_START), proto::RefereeState_Command_KICKOFF_US},
        {refPair(proto::Referee_Command_PREPARE_KICKOFF_YELLOW,proto::Referee_Command_NORMAL_START), proto::RefereeState_Command_KICKOFF_THEM},
        {refPair(proto::Referee_Command_PREPARE_PENALTY_BLUE,proto::Referee_Command_NORMAL_START), proto::RefereeState_Command_PENALTY_US},
        {refPair(proto::Referee_Command_PREPARE_PENALTY_YELLOW,proto::Referee_Command_NORMAL_START), proto::RefereeState_Command_PENALTY_THEM},

};
std::optional<proto::RefereeState_Command> getCommand(proto::Referee_Command oldCommand, proto::Referee_Command newCommand) {
    auto it=commandSwitch.find(refPair(oldCommand,newCommand));
    if(it==commandSwitch.end()){
        return std::nullopt;
    }
    return it->second;
}
proto::RefereeState_Command defaultMap(proto::Referee_Command command){
    switch(command){
    case proto::Referee_Command_HALT: return proto::RefereeState_Command_HALT;
    case proto::Referee_Command_STOP: return proto::RefereeState_Command_STOP;
    case proto::Referee_Command_NORMAL_START: return proto::RefereeState_Command_RUNNING;
    case proto::Referee_Command_FORCE_START: return proto::RefereeState_Command_RUNNING; // perhaps add force start to something?
    case proto::Referee_Command_PREPARE_KICKOFF_YELLOW: return proto::RefereeState_Command_PREPARE_KICKOFF_THEM;
    case proto::Referee_Command_PREPARE_KICKOFF_BLUE: return proto::RefereeState_Command_PREPARE_KICKOFF_US;
    case proto::Referee_Command_PREPARE_PENALTY_YELLOW: return proto::RefereeState_Command_PREPARE_PENALTY_THEM;
    case proto::Referee_Command_PREPARE_PENALTY_BLUE: return proto::RefereeState_Command_PREPARE_PENALTY_US;
    case proto::Referee_Command_DIRECT_FREE_YELLOW: return proto::RefereeState_Command_DIRECT_FREE_THEM;
    case proto::Referee_Command_DIRECT_FREE_BLUE: return proto::RefereeState_Command_DIRECT_FREE_US;
    case proto::Referee_Command_INDIRECT_FREE_YELLOW: return proto::RefereeState_Command_INDIRECT_FREE_THEM;
    case proto::Referee_Command_INDIRECT_FREE_BLUE: return proto::RefereeState_Command_INDIRECT_FREE_US;
    case proto::Referee_Command_TIMEOUT_YELLOW: return proto::RefereeState_Command_TIMEOUT_THEM;
    case proto::Referee_Command_TIMEOUT_BLUE: return proto::RefereeState_Command_TIMEOUT_US;
    case proto::Referee_Command_GOAL_YELLOW: return proto::RefereeState_Command_STOP; //see proto file. These are deprecated and must be treated as stop
    case proto::Referee_Command_GOAL_BLUE: return proto::RefereeState_Command_STOP;
    case proto::Referee_Command_BALL_PLACEMENT_YELLOW: return proto::RefereeState_Command_BALL_PLACEMENT_THEM;
    case proto::Referee_Command_BALL_PLACEMENT_BLUE: return proto::RefereeState_Command_BALL_PLACEMENT_US;
    }
}
proto::RefereeState_Command invertTeams(proto::RefereeState_Command command){
    switch(command){
    case proto::RefereeState_Command_HALT: return proto::RefereeState_Command_HALT;
    case proto::RefereeState_Command_STOP: return proto::RefereeState_Command_STOP;
    case proto::RefereeState_Command_RUNNING: return proto::RefereeState_Command_RUNNING;
    case proto::RefereeState_Command_PREPARE_KICKOFF_THEM: return proto::RefereeState_Command_PREPARE_KICKOFF_US;
    case proto::RefereeState_Command_PREPARE_KICKOFF_US: return proto::RefereeState_Command_PREPARE_KICKOFF_THEM;
    case proto::RefereeState_Command_PREPARE_PENALTY_THEM: return proto::RefereeState_Command_PREPARE_PENALTY_US;
    case proto::RefereeState_Command_PREPARE_PENALTY_US: return proto::RefereeState_Command_PREPARE_PENALTY_THEM;
    case proto::RefereeState_Command_DIRECT_FREE_THEM: return proto::RefereeState_Command_DIRECT_FREE_US;
    case proto::RefereeState_Command_DIRECT_FREE_US: return proto::RefereeState_Command_DIRECT_FREE_THEM;
    case proto::RefereeState_Command_INDIRECT_FREE_THEM: return proto::RefereeState_Command_INDIRECT_FREE_US;
    case proto::RefereeState_Command_INDIRECT_FREE_US: return proto::RefereeState_Command_INDIRECT_FREE_THEM;
    case proto::RefereeState_Command_TIMEOUT_THEM: return proto::RefereeState_Command_TIMEOUT_US;
    case proto::RefereeState_Command_TIMEOUT_US: return proto::RefereeState_Command_TIMEOUT_THEM;
    case proto::RefereeState_Command_BALL_PLACEMENT_THEM: return proto::RefereeState_Command_BALL_PLACEMENT_US;
    case proto::RefereeState_Command_BALL_PLACEMENT_US: return proto::RefereeState_Command_BALL_PLACEMENT_THEM;
    case proto::RefereeState_Command_KICKOFF_US: return proto::RefereeState_Command_KICKOFF_THEM;
    case proto::RefereeState_Command_KICKOFF_THEM: return proto::RefereeState_Command_KICKOFF_US;
    case proto::RefereeState_Command_PENALTY_US: return proto::RefereeState_Command_PENALTY_THEM;
    case proto::RefereeState_Command_PENALTY_THEM: return proto::RefereeState_Command_PENALTY_US;
    }
}