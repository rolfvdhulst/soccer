//
// Created by rolf on 17-11-19.
//

#include "ball/BallFilter.h"
#include "Scaling.h"
#include "FilterConstants.h"
#include <visionMatlab/VisionMatlabLogger.h>

BallFilter::BallFilter(const BallObservation& observation)  :
ObjectFilter(0.2,1/60.0,15,3,observation.timeCaptured)
{
    // SSL units are in mm, we do everything in SI units.
    double x = mmToM(observation.ball.x());  // m
    double y = mmToM(observation.ball.y());  // m
    Eigen::Vector4d startState = {x, y, 0, 0};

    Eigen::Matrix4d startCovariance = Eigen::Matrix4d::Zero();
    startCovariance(0,0) = BALL_POSITION_INITIAL_COV;
    startCovariance(1,1) = BALL_POSITION_INITIAL_COV;
    startCovariance(2,2) = BALL_VELOCITY_INITIAL_COV;
    startCovariance(3,3) = BALL_VELOCITY_INITIAL_COV;

    positionFilter = PosVelFilter2D(startState,startCovariance,BALL_POSITION_MODEL_ERROR,BALL_POSITION_MEASUREMENT_ERROR,observation.timeCaptured);
    registerLogFile({x,y});
}

bool BallFilter::justUpdated() const {
    return lastCycleWasUpdate;
}

void BallFilter::predict(Time time) {
    positionFilter.predict(time);
    lastCycleWasUpdate = false;
}
bool BallFilter::update(const BallObservation &observation) {
    Eigen::Vector2d detectedPos = {mmToM(observation.ball.x()),mmToM(observation.ball.y())};
    if((detectedPos-positionFilter.getPosition()).squaredNorm()>=0.5*0.5){
        return false;
    }
    positionFilter.update(detectedPos);
    objectSeen(observation.timeCaptured);
    lastCycleWasUpdate = true;
    writeLogFile(detectedPos);
    return true;
}

void BallFilter::updateBallNotSeen(const Time &time) {
    objectInvisible(time);
}

FilteredBall BallFilter::getEstimate(const Time &time, bool writeUncertainties) const {
    FilteredBall ball;
    ball.pos = positionFilter.getPositionEstimate(time);
    ball.vel = positionFilter.getVelocity();
    ball.isVisible = time-lastSeen()>Time(0.05);
    if(writeUncertainties){
        ball.health = getHealth();
        ball.posUncertainty = positionFilter.getPositionUncertainty().norm();
        ball.velocityUncertainty = positionFilter.getVelocityUncertainty().norm();
    }
    return ball;
}

void BallFilter::writeLogFile(const Eigen::Vector2d& observation) {
    if(!matlab_logger::logger.isCurrentlyLogging()){
        return;
    }
    int id = getID();
    if(id>0){
        matlab_logger::logger.writeData(
                id,
                positionFilter.lastUpdated().asSeconds(),
                observation,
                positionFilter.getState(),
                positionFilter.getCovariance(),
                positionFilter.getInnovation()
        );
    } else{
        registerLogFile(observation);
    }

}
void BallFilter::registerLogFile(const Eigen::Vector2d &observation) {
    if(!matlab_logger::logger.isCurrentlyLogging()){
        return;
    }
    int id = matlab_logger::logger.writeNewFilter(4,2,VisionMatlabLogger::BALL_FILTER);
    setID(id);
    writeLogFile(observation);
}
