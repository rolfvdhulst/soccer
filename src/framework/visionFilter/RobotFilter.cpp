//
// Created by rolf on 05-11-19.
//

#include "RobotFilter.h"
#include "Scaling.h"
#include "FilterConstants.h"

RobotFilter::RobotFilter(const RobotObservation& observation) :
ObjectFilter(0.2,1/60.0,10,3),
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
    if((detectedPos-positionFilter.getPosition()).norm()>=0.4 || angleDif > M_PI_2){
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

proto::WorldRobot RobotFilter::asWorldRobot(const Time &time) const {
    proto::WorldRobot msg;
    //Note we do linear extrapolation in the future for robots we don't know yet!
    const Eigen::Vector2d pos = positionFilter.getPositionEstimate(time);
    const Eigen::Vector2d vel = positionFilter.getVelocity();
    const double angle = angleFilter.getPositionEstimate(time);
    const double angularVel = angleFilter.getVelocity();
    msg.set_id(botId);
    msg.mutable_pos()->set_x(pos.x());
    msg.mutable_pos()->set_y(pos.y());
    msg.set_angle(angle);  // Need to limit here again (see applyObservation)
    msg.mutable_vel()->set_x(vel.x());
    msg.mutable_vel()->set_y(vel.y());
    msg.set_w(angularVel);
    return msg;
}

bool RobotFilter::justUpdated() const {
    return lastCycleWasUpdate;
}

