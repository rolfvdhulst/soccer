//
// Created by rolf on 30-03-20.
//

#ifndef SOCCER_COMMANDSWITCH_H
#define SOCCER_COMMANDSWITCH_H

#include <protobuf/GameState.pb.h>
#include <optional>

std::optional<proto::RefereeState_Command> getCommand(proto::Referee_Command oldCommand,proto::Referee_Command newCommand);

proto::RefereeState_Command defaultMap(proto::Referee_Command command);
proto::RefereeState_Command invertTeams(proto::RefereeState_Command command);


#endif //SOCCER_COMMANDSWITCH_H
