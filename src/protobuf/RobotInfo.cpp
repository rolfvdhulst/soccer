//
// Created by rolf on 04-04-20.
//

#include "RobotInfo.h"
#include <cmath>
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
    float radius=0.09;
    robot.set_radius(radius);
    float dribblerAngle = 0.785; //45 degrees roughly
    robot.set_frontangle(dribblerAngle);
    robot.set_frontwidth(sinf(dribblerAngle)*radius*2);
    robot.set_centertofrontdist(cosf(dribblerAngle)*radius);
    robot.set_dribblerwidth(robot.frontwidth());
    robot.set_centertodribblerdist(robot.centertofrontdist());
    robot.set_angleoffset(0.0);
    return robot;
}
proto::RobotInfo RTT_ROBOT_2019(){
    proto::RobotInfo robot;
    robot.set_height(0.147);
    float radius=0.089;
    robot.set_radius(radius);
    float dribblerAngle = 0.7247; //41.52 degrees roughly
    robot.set_frontangle(dribblerAngle);
    robot.set_frontwidth(sinf(dribblerAngle)*radius*2);
    robot.set_centertofrontdist(cosf(dribblerAngle)*radius);
    robot.set_dribblerwidth(robot.frontwidth()); //TODO: figure these out for our real robots
    robot.set_centertodribblerdist(robot.centertofrontdist());
    robot.set_angleoffset(0.0);
    return robot;
}