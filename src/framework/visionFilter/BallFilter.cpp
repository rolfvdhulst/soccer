//
// Created by rolf on 05-08-20.
//

#include "BallFilter.h"


BallFilter::BallFilter(const BallObservation &observation) : ObjectFilter(),
        cameraFilters{std::make_pair(observation.cameraID, CameraBallFilter(observation))} {

}

void BallFilter::predictCam(const int &cameraID, const Time &untilTime) {
    auto cameraFilter = cameraFilters.find(cameraID);
    if (cameraFilter != cameraFilters.end()) {
        cameraFilter->second.predict(untilTime);
    }
}

bool BallFilter::processNotSeen(const int &cameraID, const Time &time) {
    auto cameraFilter = cameraFilters.find(cameraID);
    if (cameraFilter == cameraFilters.end()) {
        return false; //if the relevant camera does not exist, we do not need to remove it
    }
    if (!cameraFilter->second.justUpdated()) {
        bool removeFilter = cameraFilter->second.updateBallNotSeen(time);
        if (removeFilter) {
            cameraFilters.erase(cameraFilter);
        }
    }
    return cameraFilters.empty();
}

bool BallFilter::processDetection(const BallObservation &observation) {
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
            Eigen::Vector2d velocity{0, 0};
            for (const auto &filter : cameraFilters) {
                velocity += filter.second.getVelocity(observation.timeCaptured);
            }
            velocity /= cameraFilters.size();
            cameraFilters.insert(std::make_pair(observation.cameraID, CameraBallFilter(observation,velocity)));
        }
        return accept;
    }
}
