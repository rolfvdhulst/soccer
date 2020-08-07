//
// Created by rolf on 05-08-20.
//

#include "RobotFilter.h"

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

bool RobotFilter::processNotSeen(const int &cameraID, const Time &time) {
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

