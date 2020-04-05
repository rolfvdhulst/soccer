//
// Created by rolf on 04-04-20.
//

#ifndef SOCCER_ROBOTINFO_H
#define SOCCER_ROBOTINFO_H

#include "protobuf/RobotInfo.pb.h"

proto::RobotInfo getTeamInfo(const std::string& team);
proto::RobotInfo DEFAULT_ROBOT();
proto::RobotInfo RTT_ROBOT_2019();

#endif //SOCCER_ROBOTINFO_H
