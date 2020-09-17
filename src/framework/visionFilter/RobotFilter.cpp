//
// Created by rolf on 05-08-20.
//

#include "RobotFilter.h"
#include <math/geometry/Angle.h>

bool RobotFilter::processDetection(const RobotObservation &observation) {
    auto cameraFilter = cameraFilters.find(observation.cameraID);
    if (cameraFilter != cameraFilters.end()) {
        bool accept = cameraFilter->second.acceptObservation(observation);
        if (accept) {
            cameraFilter->second.update(observation);
        }
        return accept;
    } else {
        assert(!cameraFilters.empty());
        //TODO: figure out how to write prettily using std::all_of.
        //TODO: also, maybe use std::any_of here or some other criterion (e.g. it's okay if one filter does not accept it?)
        bool accept = true;
        for (const auto &filter : cameraFilters) {
            accept &= filter.second.acceptObservation(observation);
        }
        if (accept) {
            //We can initialize this new filter with information from the other filters, by giving it the initial speed the others detected
            //TODO: make this a weighted average (using e.g. filter age / health?)
            Eigen::Vector3d velocity{0, 0, 0};
            for (const auto &filter : cameraFilters) {
                velocity += filter.second.getVelocity(observation.timeCaptured);
            }
            velocity /= cameraFilters.size();
            cameraFilters.insert(std::make_pair(observation.cameraID, CameraRobotFilter(observation, isBlue,velocity)));
        }
        return accept;
    }
}

RobotFilter::RobotFilter(const RobotObservation &observation, bool botIsBlue) :
        ObjectFilter(),
        isBlue{botIsBlue},
        robotID{observation.robotId},
        cameraFilters{std::make_pair(observation.cameraID, CameraRobotFilter(observation, botIsBlue))} {
}

bool RobotFilter::processNotSeen(int cameraID, const Time &time) {
    auto cameraFilter = cameraFilters.find(cameraID);
    if (cameraFilter == cameraFilters.end()) {
        return false; //if the relevant camera does not exist, we do not need to remove it
    }
    if (!cameraFilter->second.justUpdated()) {
        bool removeFilter = cameraFilter->second.updateRobotNotSeen(time);
        if (removeFilter) {
            cameraFilters.erase(cameraFilter);
        }
    }
    return cameraFilters.empty();
}

void RobotFilter::predictCam(const int &cameraID, const Time &untilTime) {
    auto cameraFilter = cameraFilters.find(cameraID);
    if (cameraFilter != cameraFilters.end()) {
        cameraFilter->second.predict(untilTime);
    }
}
double RobotFilter::getHealth() const {
    double maxHealth = 0.0;
    for (const auto &filter : cameraFilters) {
        maxHealth = fmax(filter.second.getHealth(), maxHealth);
    }
    return maxHealth;
}
FilteredRobot RobotFilter::mergeRobots(const Time &time) const {
    double mergeFactor = 1.5;
    Eigen::Vector2d vel(0, 0);
    Eigen::Vector2d pos(0, 0);
    double angle = 0;
    double angularVel = 0;
    double totalPosUncertainty = 0;
    double totalVelUncertainty = 0;
    double totalAngleUncertainty = 0;
    double totalAngleVelUncertainty = 0;
    //We cannot take averages of angular coordinates easily, so we take the averages of the offsets (this works)
    double angleOffset = cameraFilters.begin()->second.getEstimate(time).angle;

    for (const auto &filter : cameraFilters) {
        FilteredRobot robot = filter.second.getEstimate(time, true);
        //Use the filter health and uncertainties for a weighted average of observations
        double weight = 100.0/robot.health;
        double posWeight = pow(robot.posUncertainty*weight, - mergeFactor);
        double velWeight = pow(robot.velocityUncertainty*weight, - mergeFactor);
        double angleWeight = pow(robot.angleUncertainty*weight,-mergeFactor);
        double angleVelWeight = pow(robot.angularVelUncertainty*weight,-mergeFactor);

        pos += robot.pos*posWeight;
        vel += robot.vel*velWeight;
        angle += Angle(robot.angle - angleOffset).getAngle()*angleWeight;
        angularVel += robot.angularVel*angleVelWeight;

        totalPosUncertainty += posWeight;
        totalVelUncertainty += velWeight;
        totalAngleUncertainty += angleWeight;
        totalAngleVelUncertainty += angleVelWeight;

    }
    pos /= totalPosUncertainty;
    vel /= totalVelUncertainty;
    angle /= totalAngleUncertainty;
    angularVel /= totalAngleVelUncertainty;
    FilteredRobot result;
    result.pos = pos;
    result.vel = vel;
    result.angle = Angle(angleOffset+angle).getAngle();
    result.angularVel = angularVel;
    result.id = robotID;
    result.isBlue = isBlue;
    return result;
}

std::optional<RobotTrajectorySegment> RobotFilter::getLastFrameTrajectory(int cameraID, const RobotParameters &parameters) const {
    auto cameraFilter = cameraFilters.find(cameraID);
    if(cameraFilter != cameraFilters.end()){
        return cameraFilter->second.getFrameTrajectory(parameters);
    }else{
        return std::nullopt;
    }
}

std::optional<FilteredRobot> RobotFilter::getRobot(int cameraID, Time time) const {
    auto cameraFilter = cameraFilters.find(cameraID);
    if(cameraFilter != cameraFilters.end()){
        return cameraFilter->second.getEstimate(time,true);
    }else{
        return std::nullopt;
    }
}


