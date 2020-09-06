//
// Created by rolf on 05-11-19.
//

#include "CameraRobotFilter.h"
#include "FilterConstants.h"
#include <visionMatlab/VisionMatlabLogger.h>

CameraRobotFilter::CameraRobotFilter(const RobotObservation& observation, bool botIsBlue, Eigen::Vector3d velocityEstimate) :
        CameraObjectFilter(0.2, 1 / 60.0, 10, 3, observation.timeCaptured),
        botId{observation.robotId},
        cameraID{observation.cameraID},
        botIsBlue{botIsBlue}{
    //Initialize position filter
    //TODO: initialize from other camera
    Eigen::Matrix4d initialPosCov = Eigen::Matrix4d::Zero();
    initialPosCov(0,0) = ROBOT_POSITION_INITIAL_COV;
    initialPosCov(1,1) = ROBOT_POSITION_INITIAL_COV;
    initialPosCov(2,2) = ROBOT_VELOCITY_INITIAL_COV;
    initialPosCov(3,3) = ROBOT_VELOCITY_INITIAL_COV;

    Eigen::Vector4d initialPos ={observation.position.x(),observation.position.y(),velocityEstimate.x(),velocityEstimate.y()};
    positionFilter = PosVelFilter2D(initialPos,initialPosCov,ROBOT_POSITION_MODEL_ERROR,ROBOT_POSITION_MEASUREMENT_ERROR,observation.timeCaptured);

    Eigen::Vector2d initialAngle = {observation.orientation,velocityEstimate.z()};
    Eigen::Matrix2d initialAngleCov = Eigen::Matrix2d::Zero();
    initialAngleCov(0,0) = ROBOT_ANGLE_INITIAL_COV;
    initialAngleCov(1,1) = ROBOT_ANGULAR_VEL_INITIAL_COV;
    angleFilter = RobotOrientationFilter(initialAngle,initialAngleCov,ROBOT_ANGLE_MODEL_ERROR,ROBOT_ANGLE_MEASUREMENT_ERROR,observation.timeCaptured);

    previousPos = observation.position;
    previousAngle = Angle(observation.orientation);
    previousTime = Time(observation.timeCaptured);

    registerLogFile(observation.position,observation.orientation);
}

void CameraRobotFilter::predict(Time time) {
    updatePreviousInfo();
    positionFilter.predict(time);
    angleFilter.predict(time);
    lastCycleWasUpdate = false;
}

bool CameraRobotFilter::update(const RobotObservation &observation) {
    assert(observation.robotId == botId); //sanity check
    assert(observation.cameraID == cameraID);
    //Update position kalman filter
    positionFilter.update(observation.position);
    //Update angle kalman filter
    angleFilter.update(observation.orientation);
    //update object seen settings
    objectSeen(observation.timeCaptured);
    lastCycleWasUpdate = true;
    writeLogFile(observation.position,observation.orientation);
    return true;
}

bool CameraRobotFilter::updateRobotNotSeen(const Time &time) {
    objectInvisible(time);
    return getHealth() <= 0.0 && consecutiveFramesNotSeen() > 3; // We remove a robot if it's health is zero and it has not been seen for 3 frames
}


bool CameraRobotFilter::justUpdated() const {
    return lastCycleWasUpdate;
}

FilteredRobot CameraRobotFilter::getEstimate(const Time &time, bool writeUncertainties) const {
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

void CameraRobotFilter::registerLogFile(const Eigen::Vector2d &observation, double angleObserved) {
    if(!matlab_logger::logger.isCurrentlyLogging()){
        return;
    }
    VisionMatlabLogger::FilterType type = botIsBlue ? VisionMatlabLogger::ROBOT_FILTER_POSITION_BLUE : VisionMatlabLogger::ROBOT_FILTER_POSITION_YELLOW;
    int positionFilterID = matlab_logger::logger.writeNewFilter(4,2,type);
    setID(positionFilterID);
    type = botIsBlue ? VisionMatlabLogger::ROBOT_FILTER_ANGLE_BLUE : VisionMatlabLogger::ROBOT_FILTER_ANGLE_YELLOW;
    orientationFilterUniqueId = matlab_logger::logger.writeNewFilter(2,1,type);
    writeLogFile(observation,angleObserved);
}
void CameraRobotFilter::writeLogFile(const Eigen::Vector2d &observation, double observedAngle) {
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
        Eigen::Matrix<double,1,1> angleSeen = {Eigen::Matrix<double,1,1>(observedAngle)};
        Eigen::Matrix<double,1,1> innovation = {Eigen::Matrix<double,1,1>(angleFilter.getInnovation())};
        matlab_logger::logger.writeData(
                orientationFilterUniqueId,
                angleFilter.lastUpdated().asSeconds(),
                angleSeen,
                angleFilter.getState(),
                angleFilter.getCovariance(),
                innovation
                );
    } else{
        registerLogFile(observation,observedAngle);
    }
}

bool CameraRobotFilter::acceptObservation(const RobotObservation& observation) const{
    double angleDif = abs(RobotOrientationFilter::limitAngle(angleFilter.getPosition()-observation.orientation));
    double posDifSq = (observation.position-positionFilter.getPosition()).squaredNorm();
    return posDifSq<0.4*0.4 && angleDif < M_PI_2;
}

Eigen::Vector3d CameraRobotFilter::getVelocity(const Time &time) const {
    Eigen::Vector2d vel = positionFilter.getVelocity();
    double angVel = angleFilter.getVelocity();
    return Eigen::Vector3d(vel.x(),vel.y(),angVel);
}
RobotTrajectorySegment CameraRobotFilter::getFrameTrajectory(const RobotParameters &robotParams) const{
    Vector2 currentPos(positionFilter.getPosition());
    Angle currentAngle(angleFilter.getPosition());
    Time currentTime = positionFilter.lastUpdated();

    double dt = (currentTime-previousTime).asSeconds();
    double w = (currentAngle-previousAngle).getAngle()/dt;
    Vector2 vel = (currentPos-previousPos)/dt;
    if(dt==0.0){ //This happens if robot was just initialized
        w = 0.0;
        vel = Vector2();
    }

    RobotTrajectorySegment segment;
    segment.startPos = RobotShape(previousPos,robotParams.centerToFront,robotParams.radius,previousAngle);//Contains initial angle and position
    segment.vel = vel;
    segment.angVel = w;
    segment.dt = dt;
    segment.startTime = currentTime;
    segment.endTime = previousTime;
    segment.isBlue = botIsBlue;
    segment.robotID = botId;
    return segment;
}

void CameraRobotFilter::updatePreviousInfo() {
    previousPos = positionFilter.getPosition();
    previousAngle = angleFilter.getPosition();
    previousTime = positionFilter.lastUpdated();
}