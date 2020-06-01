//
// Created by rolf on 05-11-19.
//

#include "RobotFilter.h"
#include "Scaling.h"
#include "FilterConstants.h"

RobotFilter::RobotFilter(const RobotObservation& observation) :
ObjectFilter(0.2,1/60.0,10,3,observation.timeCaptured),
botId{static_cast<int>(observation.bot.robot_id())}{
    //Initialize position filter
    //TODO: initialize from other camera
    Eigen::Matrix4d initialPosCov = Eigen::Matrix4d::Zero();
    initialPosCov(0,0) = ROBOT_POSITION_INITIAL_COV;
    initialPosCov(1,1) = ROBOT_POSITION_INITIAL_COV;
    initialPosCov(2,2) = ROBOT_VELOCITY_INITIAL_COV;
    initialPosCov(3,3) = ROBOT_VELOCITY_INITIAL_COV;

    Eigen::Vector4d initialPos ={mmToM(observation.bot.x()),mmToM(observation.bot.y()),0,0};
    positionFilter = PosVelFilter2D(initialPos,initialPosCov,ROBOT_POSITION_MODEL_ERROR,ROBOT_POSITION_MEASUREMENT_ERROR,observation.timeCaptured);

    Eigen::Vector2d initialAngle = {observation.bot.orientation(),0};
    Eigen::Matrix2d initialAngleCov = Eigen::Matrix2d::Zero();
    initialAngleCov(0,0) = ROBOT_ANGLE_INITIAL_COV;
    initialAngleCov(1,1) = ROBOT_ANGULAR_VEL_INITIAL_COV;
    angleFilter = RobotOrientationFilter(initialAngle,initialAngleCov,ROBOT_ANGLE_MODEL_ERROR,ROBOT_ANGLE_MEASUREMENT_ERROR,observation.timeCaptured);

}

void RobotFilter::predict(Time time) {
    positionFilter.predict(time);
    angleFilter.predict(time);
    lastCycleWasUpdate = false;
}

bool RobotFilter::update(const RobotObservation &observation) {
    assert(observation.bot.robot_id() == botId); //sanity check
    Eigen::Vector2d detectedPos = {mmToM(observation.bot.x()),mmToM(observation.bot.y())};
    double angleDif = abs(RobotOrientationFilter::limitAngle(angleFilter.getPosition()-observation.bot.orientation()));
    if((detectedPos-positionFilter.getPosition()).squaredNorm()>=0.4*0.4 || angleDif > M_PI_2){
        return false;
    }
    //Update position kalman filter
    positionFilter.update(detectedPos);
    //Update angle kalman filter
    angleFilter.update(observation.bot.orientation());
    //update object seen settings
    objectSeen(observation.timeCaptured);
    lastCycleWasUpdate = true;
    return true;
}

void RobotFilter::updateRobotNotSeen(const Time &time) {
    objectInvisible(time);
}


bool RobotFilter::justUpdated() const {
    return lastCycleWasUpdate;
}

FilteredRobot RobotFilter::getEstimate(const Time &time, bool writeUncertainties) const {
    FilteredRobot robot;
    robot.id = botId;
    robot.pos = positionFilter.getPositionEstimate(time);
    robot.vel = positionFilter.getVelocity();
    robot.angle = angleFilter.getPositionEstimate(time);
    robot.angularVel = angleFilter.getVelocity();
    if(writeUncertainties){
        robot.health = getHealth();
        robot.posUncertainty = positionFilter.getPositionUncertainty().norm();
        robot.velocityUncertainty = positionFilter.getVelocityUncertainty().norm();
        robot.angleUncertainty = angleFilter.getPositionUncertainty();
        robot.angularVelUncertainty = angleFilter.getVelocityUncertainty();
    }
    return robot;
}

