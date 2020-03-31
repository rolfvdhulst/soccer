//
// Created by rolf on 30-03-20.
//

#ifndef SOCCER_COMMANDSWITCH_H
#define SOCCER_COMMANDSWITCH_H

#include <protobuf/GameState.pb.h>
#include <optional>

std::optional<proto::GameState_Command> getCommand(proto::Referee_Command oldCommand,proto::Referee_Command newCommand);

proto::GameState_Command defaultMap(proto::Referee_Command command);
proto::GameState_Command invertTeams(proto::GameState_Command command);


#endif //SOCCER_COMMANDSWITCH_H
