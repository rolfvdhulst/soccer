//
// Created by rolf on 04-04-20.
//

#include "RobotInfo.h"

proto::RobotInfo getTeamInfo(const std::string& teamName){
    if(teamName == "RoboTeam Twente"){
        return RTT_ROBOT_2019();
    }
    return DEFAULT_ROBOT();
}
//TODO: fill fields and correct values
proto::RobotInfo DEFAULT_ROBOT(){
    proto::RobotInfo robot;
    robot.set_height(0.15);
    robot.set_radius(0.09);
    return robot;
}
proto::RobotInfo RTT_ROBOT_2019(){
    proto::RobotInfo robot;
    robot.set_height(0.15);
    robot.set_radius(0.09);
    return robot;
}