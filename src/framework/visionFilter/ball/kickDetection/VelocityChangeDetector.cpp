//
// Created by rolf on 28-07-20.
//

#include "VelocityChangeDetector.h"

#include <math/geometry/Vector2.h>
#include <math/geometry/Angle.h>

bool VelocityChangeDetector::velocityChanged() const {
    const BallObservation &first =  buffer.at(0);
    const BallObservation &second = buffer.at(1);
    const BallObservation &third = buffer.at(2);

    Vector2 firstPos(first.ball.x(), first.ball.y());
    Vector2 secondPos(second.ball.x(), second.ball.y());
    Vector2 thirdPos(third.ball.x(), third.ball.y());

    Vector2 firstVel = (secondPos - firstPos) / (second.timeCaptured - first.timeCaptured).asSeconds();
    Vector2 secondVel = (thirdPos - secondPos) / (third.timeCaptured - second.timeCaptured).asSeconds();

    //Check if velocity changed significantly
    double speedDiff = (secondVel.length() - firstVel.length());
    // absolute difference in angle
    double angleDiff = abs(firstVel.toAngle().angleDiff(secondVel.toAngle()));

    constexpr double threshHold = 1000; // mm/s
    constexpr double rollingDeviation = 10; // degrees
    // Due to timing inaccuracies some camera's may give a wrong time stamp to a detection or be slightly inaccurate.
    // If the ball is rolling fast this results in invalid speed estimations.
    // If the ball is still on the same line (within 10 radians), and was already rolling faster than the threshhold
    // we still assume that the ball is NOT kicked because of this. Otherwise speed oscilations will lead to invalid kick detections
    // Perhaps we can use this in the future to mark detection frames as having an 'inaccurate' time stamp
    if (speedDiff > threshHold && (angleDiff > toRadians(rollingDeviation) || firstVel.length() < threshHold)) {
        return true;
    }
    constexpr double velThreshHold = 1000; //mm/s
    constexpr double kickDeviation = 20; //degrees
    //This is more effective at catching redirects and especially rebounds.
    return firstVel.length() > velThreshHold && secondVel.length() > velThreshHold &&
           angleDiff > toRadians(kickDeviation);
}

bool VelocityChangeDetector::detectKick(const BallObservation &observation) {
    if(!buffer.empty() && observation.timeCaptured < buffer.back().timeCaptured) {
        throw std::invalid_argument("invalid timestamp");
    }
    ++observations;
    buffer.push_back(observation);
    if(!buffer.full()){
        return false;
    }
    Time middleTime = buffer.at(1).timeCaptured;
    bool kickDetected = (middleTime-lastChangedTime) > COOLDOWN_TIME && velocityChanged();
    if(kickDetected){
        lastChangedTime = middleTime;
    }
    return kickDetected;
}

