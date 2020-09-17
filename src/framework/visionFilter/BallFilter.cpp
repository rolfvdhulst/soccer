//
// Created by rolf on 05-08-20.
//

#include "BallFilter.h"

BallFilter::BallFilter(const BallObservation &observation)
        : ObjectFilter(),
          cameraFilters{std::make_pair(observation.cameraID, CameraBallGroundEKF(observation))} {
    acceptedBalls.push_back(observation);
}

void BallFilter::predictCam(int cameraID, const Time &untilTime, const GeometryData &geometryData,
                            const std::vector<RobotTrajectorySegment>& robotTrajectories){
    auto cameraFilter = cameraFilters.find(cameraID);
    if (cameraFilter != cameraFilters.end()) {
        cameraFilter->second.predict(untilTime, geometryData,robotTrajectories);
    }
}



double BallFilter::getHealth() const {
    double maxHealth = 0.0;
    for (const auto &filter : cameraFilters) {
        maxHealth = fmax(filter.second.getHealth(), maxHealth);
    }
    return maxHealth;
}

FilteredBall BallFilter::mergeBalls(const Time &time) const {
    double mergeFactor = 1.5;
    Eigen::Vector2d vel(0, 0);
    Eigen::Vector2d pos(0, 0);
    double totalPosUncertainty = 0;
    double totalVelUncertainty = 0;
    bool isVisible = false;
    for (const auto &filter : cameraFilters) {
        FilteredBall ball = filter.second.getEstimate(time, true);
        //Use the filter health and uncertainties for a weighted average of observations
        double weight = 100.0 / ball.health;
        double posWeight = pow(ball.posUncertainty * weight, -mergeFactor);
        double velWeight = pow(ball.velocityUncertainty * weight, -mergeFactor);
        pos += ball.pos * posWeight;
        vel += ball.vel * velWeight;
        totalPosUncertainty += posWeight;
        totalVelUncertainty += velWeight;
        isVisible |= ball.isVisible;
    }
    pos /= totalPosUncertainty;
    vel /= totalVelUncertainty;
    FilteredBall result;
    result.pos = pos;
    result.vel = vel;
    result.isVisible = isVisible;
    return result;
}

bool BallFilter::processFrame(int cameraID, Time time) {
    auto cameraFilter = cameraFilters.find(cameraID);
    if (cameraFilter == cameraFilters.end()) {
        return false; //The relevant camera does not exist
    }
    //Process all the accepted detections of the frame in the filter
    bool removeFilter = cameraFilter->second.processFrame();
    if(removeFilter){
        cameraFilters.erase(cameraFilter);
    }
    return cameraFilters.empty();
}

bool BallFilter::acceptDetection(const BallObservation &observation) {
    auto cameraFilter = cameraFilters.find(observation.cameraID);
    if (cameraFilter != cameraFilters.end()){
        bool accepted = cameraFilter->second.addObservation(observation);
        acceptedBalls.push_back(observation);
        return accepted;
    }
    //Check if it makes sense that we see the relative to the other camera's
    bool accepted = true;
    for (const auto &filter : cameraFilters) {
        accepted &= filter.second.acceptObservation(observation);
    }
    if (accepted) {
        if(!cameraFilters.empty()){
            //We can initialize this new filter with information from the other filters, by giving it the initial speed the others detected
            //TODO: make this a weighted average (using e.g. filter age / health?)
            Eigen::Vector2d velocity{0, 0};
            for (const auto &filter : cameraFilters) {
                velocity += filter.second.getVelocity(observation.timeCaptured);
            }
            velocity /= cameraFilters.size();
            cameraFilters.insert(std::make_pair(observation.cameraID, CameraBallGroundEKF(observation, velocity)));
        } else{
            cameraFilters.insert(std::make_pair(observation.cameraID,CameraBallGroundEKF(observation)));
        }
        acceptedBalls.push_back(observation);
    }
    return accepted;

}

BallObservation BallFilter::lastDetection() const {
    return acceptedBalls.back();
}
