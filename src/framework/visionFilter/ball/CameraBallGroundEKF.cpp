//
// Created by rolf on 10-08-20.
//

#include "ball/CameraBallGroundEKF.h"

#include <utility>
#include <FilterConstants.h>
#include <visionMatlab/VisionMatlabLogger.h>
#include "ball/collision/CollisionChecker.h"

CameraBallGroundEKF::CameraBallGroundEKF(const BallObservation &observation, Eigen::Vector2d velocityEstimate) :
        CameraObjectFilter(0.2, 1 / 60.0, 15, 3, observation.timeCaptured) {
    Eigen::Vector4d startState = {observation.position.x(), observation.position.y(), velocityEstimate.x(),
                                  velocityEstimate.y()};

    Eigen::Matrix4d startCovariance = Eigen::Matrix4d::Zero();
    startCovariance(0, 0) = BALL_POSITION_INITIAL_COV;
    startCovariance(1, 1) = BALL_POSITION_INITIAL_COV;
    startCovariance(2, 2) = BALL_VELOCITY_INITIAL_COV;
    startCovariance(3, 3) = BALL_VELOCITY_INITIAL_COV;

    ekf = BallEKF(startState, startCovariance, 1.0, BALL_POSITION_MEASUREMENT_ERROR, observation.timeCaptured);
    registerLogFile(observation.position);

}

void CameraBallGroundEKF::registerLogFile(const Eigen::Vector2d &observation) {
    if (!matlab_logger::logger.isCurrentlyLogging()) {
        return;
    }
    int id = matlab_logger::logger.writeNewFilter(4, 2, VisionMatlabLogger::BALL_FILTER);
    setID(id);
    writeLogFile(observation);
}

void CameraBallGroundEKF::writeLogFile(const Eigen::Vector2d &observation) {
    if (!matlab_logger::logger.isCurrentlyLogging()) {
        return;
    }
    int id = getID();
    if (id > 0) {
        matlab_logger::logger.writeData(
                id,
                ekf.lastUpdated().asSeconds(),
                observation,
                ekf.state(),
                ekf.covariance(),
                ekf.innovation()
        );
    } else {
        registerLogFile(observation);
    }

}

void CameraBallGroundEKF::update(const BallObservation &observation) {
    ekf.update(observation.position);
    objectSeen(observation.timeCaptured);
    lastCycleWasUpdate = true;
    writeLogFile(observation.position);
}

bool CameraBallGroundEKF::updateBallNotSeen(const Time &time) {
    objectInvisible(time);
    return getHealth() <= 0.0 && consecutiveFramesNotSeen() > 3;
}

bool CameraBallGroundEKF::acceptObservation(const BallObservation &observation) const {
    return (observation.position - ekf.getPosition()).squaredNorm() < 0.5 * 0.5;
}

Eigen::Vector2d CameraBallGroundEKF::getVelocity(const Time &time) const {
    return ekf.getVelocityEstimate(time);
}

void CameraBallGroundEKF::predict(Time time, const GeometryData &geometryData, const std::vector<RobotTrajectorySegment>& robotTrajectories) {
    //TODO: pack this in a function
    BallTrajectorySegment segment;
    segment.startPos = Vector2(ekf.getPosition());
    segment.startTime = ekf.lastUpdated();
    segment.endPos = Vector2(ekf.getPositionEstimate(time));
    segment.endTime = time;
    segment.startVel = Vector2(ekf.getVelocity());
    segment.acc = ekf.getAcc();
    segment.dt = (segment.endTime-segment.startTime).asSeconds();
    segment.acceleration =  Vector2(ekf.getVelocity()).normalize() * ekf.getAcc(); //TODO: check sign
    if (!(segment.startPos == segment.endPos)) { //TODO: fix this ball lying still case (maybe just pass velocity 0, acc 0?)
        int maxCollisions = 3;
        int i = 0;
        while (i < maxCollisions){
            auto collision = getFirstCollision(segment,geometryData,robotTrajectories);
            if(!collision){
                break;
            }
            ekf.predict(collision->collisionTime);
            ekf.setVelocity(collision->outVelocity);
            //TODO: base this on collision type. Maybe even pass robot velocity to this to determine uncertainty
            //TODO: if ball is kicked delay velocity setting until 1 tick later?
            //ekf.addUncertainty(0.05, std::min(0.1, collisionVel * 0.1));
            std::cout << "Collision at " << collision->position << " filter state: "
                      << Vector2(ekf.getPosition())
                      << "vel: " << collision->outVelocity
                      << "type: " << (int) collision->type
                      << std::endl;

            segment.startPos = Vector2(ekf.getPosition());
            segment.startTime = ekf.lastUpdated();
            segment.endPos = Vector2(ekf.getPositionEstimate(time));
            segment.endTime = time;
            segment.startVel = Vector2(ekf.getVelocity());
            segment.acc = ekf.getAcc();
            segment.dt = (segment.endTime-segment.startTime).asSeconds();
            segment.acceleration =  Vector2(ekf.getVelocity()).normalize() * ekf.getAcc();
            ++i;
        }
    }
    ekf.predict(time);

    lastCycleWasUpdate = false;
}


FilteredBall CameraBallGroundEKF::getEstimate(const Time &time, bool writeUncertainties) const {
    FilteredBall ball;
    ball.pos = ekf.getPositionEstimate(time);
    ball.vel = ekf.getVelocityEstimate(time);
    ball.isVisible = time - lastSeen() > Time(0.05);
    if (writeUncertainties) {
        ball.health = getHealth();
        ball.posUncertainty = ekf.getPositionUncertainty().norm();
        ball.velocityUncertainty = ekf.getVelocityUncertainty().norm();
    }
    return ball;
}

void CameraBallGroundEKF::BallEKF::predict(const Time &timeStamp) {
    if (timeStamp < lastUpdateTime) {
        std::__throw_invalid_argument("Bad timestamp");
        return;
    }
    double frame_dt = (timeStamp - lastUpdateTime).asSeconds();
    if (frame_dt <= 0) {
        return;
    }
    lastUpdateTime = timeStamp;

    Eigen::Vector2d currentPos = getPosition();
    Eigen::Vector2d currentVel = getVelocity();
    double vel = currentVel.norm();
    //We need to check if the velocity does not reach 0, as at that point the ball simply lays still
    //Because of the dimples it usually lays still when it reaches ~= 0.01 cm/s but this is typically negligible
    double dt = frame_dt;
    if (vel + acc * frame_dt < 0) {
        dt = -vel / acc;
    }


    double velCubed = vel * vel * vel;
    double vysq = currentVel.y() * currentVel.y() / velCubed;
    double vxvy = -currentVel.x() * currentVel.y() / velCubed;
    double vxsq = currentVel.x() * currentVel.x() / velCubed;
    if (vel == 0.0) {
        vxsq = 0.0;
        vxvy = 0.0;
        vysq = 0.0;
    }


    F(0, 2) = dt + 0.5 * vysq * acc * dt * dt;
    F(0, 3) = 0.5 * vxvy * acc * dt * dt;
    F(1, 2) = 0.5 * vxvy * acc * dt * dt;
    F(1, 3) = dt + 0.5 * vxsq * acc * dt * dt;
    F(2, 2) = 1 + vysq * acc * dt;
    F(2, 3) = vxvy * acc * dt;
    F(3, 2) = vxvy * acc * dt;
    F(3, 3) = 1 + vxsq * acc * dt;

    setProccessNoise(frame_dt);
    if (vel != 0.0) {
        X.head<2>() = currentPos + currentVel * dt + 0.5 * currentVel / vel * acc * dt * dt;
        X.tail<2>() = currentVel + currentVel / vel * acc * dt;

    }
    P = F * P * F.transpose() + Q;

}

void CameraBallGroundEKF::BallEKF::update(const Eigen::Vector2d &observation) {
    // Compute innovation (error between measurement and state)
    y = observation - (H * X);
    // Variance of innovation
    Eigen::Matrix2d S = H * P * H.transpose() + R;
    // compute kalman gain. For small matrices, Eigen's inverse function is efficient but be careful with larger matrices (solve a system of eq's instead)
    Eigen::Matrix<double, 4, 2> K = P * H.transpose() * S.inverse();
    // update state with prediction
    X = X + K * y;
    // update covariance
    P -= K * H * P;
}

CameraBallGroundEKF::BallEKF::BallEKF(Eigen::Vector4d initialState, Eigen::Matrix4d initialCovariance,
                                      double modelError, double measurementError, const Time &timeStamp) :
        modelError{modelError},
        lastUpdateTime{timeStamp},
        X{std::move(initialState)},
        P{std::move(initialCovariance)},
        F{Eigen::Matrix4d::Identity()},
        H{Eigen::Matrix<double, 2, 4>::Identity()},
        Q{Eigen::Matrix4d::Zero()},
        R{Eigen::Matrix2d::Identity() * measurementError},
        y{Eigen::Vector2d::Zero()} {}

Eigen::Vector2d CameraBallGroundEKF::BallEKF::getPosition() const {
    return X.head<2>();
}

Eigen::Vector2d CameraBallGroundEKF::BallEKF::getVelocityUncertainty() const {
    return P.diagonal().tail<2>().array().sqrt();
}

Eigen::Vector2d CameraBallGroundEKF::BallEKF::getVelocity() const {
    return X.tail<2>();
}

void CameraBallGroundEKF::BallEKF::setVelocity(const Eigen::Vector2d &velocity) {
    X.tail<2>() = velocity;
}

void CameraBallGroundEKF::BallEKF::addUncertainty(double posUncertainty, double velUncertainty) {
    P.diagonal().head<2>().array() += posUncertainty;
    P.diagonal().tail<2>().array() += velUncertainty;
}

Eigen::Vector2d CameraBallGroundEKF::BallEKF::getPositionUncertainty() const {
    return P.diagonal().head<2>().array().sqrt();
}

double CameraBallGroundEKF::BallEKF::getAcc() const {
    return acc;
}

void CameraBallGroundEKF::BallEKF::setAcc(double acceleration) {
    acc = acceleration;
}

void CameraBallGroundEKF::BallEKF::setProccessNoise(double dt) {
    double sigma = modelError;
    double dt3 = (1.0 / 3.0) * dt * dt * dt * sigma * sigma;
    double dt2 = (1.0 / 2.0) * dt * dt * sigma * sigma;
    double dt1 = dt * sigma * sigma;

    Q(0, 0) = dt3;
    Q(0, 2) = dt2;
    Q(1, 1) = dt3;
    Q(1, 3) = dt2;

    Q(2, 0) = dt2;
    Q(2, 2) = dt1;
    Q(3, 1) = dt2;
    Q(3, 3) = dt1;
}

Eigen::Vector4d CameraBallGroundEKF::BallEKF::getStateEstimate(double dt) const {
    Eigen::Vector2d currentPos = getPosition();
    Eigen::Vector2d currentVel = getVelocity();
    double vel = currentVel.norm();
    Eigen::Vector4d estimate;
    if (vel != 0.0) {
        estimate.head<2>() = currentPos + currentVel * dt + 0.5 * currentVel / vel * acc * dt * dt;
        estimate.tail<2>() = currentVel + currentVel / vel * acc * dt;
    } else {
        estimate.head<2>() = currentPos;
        estimate.tail<2>() = currentVel;
    }

    return estimate;
}

Eigen::Vector4d CameraBallGroundEKF::BallEKF::getStateEstimate(const Time &time) const {
    if (time < lastUpdateTime) {
        std::cout << "Too late by: " << (lastUpdateTime - time).asMilliSeconds() << " ms" << std::endl;
        std::__throw_invalid_argument("Bad timestamp");
    }
    double frame_dt = (time - lastUpdateTime).asSeconds();
    if (frame_dt <= 0) {
        return X;
    }
    double vel = getVelocity().norm();
    //We need to check if the velocity does not reach 0, as at that point the ball simply lays still
    //Because of the dimples it usually lays still when it reaches ~= 0.01 cm/s but this is typically negligible
    double dt = frame_dt;
    if (vel + acc * frame_dt < 0) {
        dt = -vel / acc;
    }
    return getStateEstimate(dt);
}

Eigen::Vector2d CameraBallGroundEKF::BallEKF::getVelocityEstimate(const Time &time) const {
    return getStateEstimate(time).tail<2>();
}

Eigen::Vector2d CameraBallGroundEKF::BallEKF::getPositionEstimate(const Time &time) const {
    return getStateEstimate(time).head<2>();
}

Eigen::Vector2d CameraBallGroundEKF::BallEKF::innovation() const {
    return y;
}

Time CameraBallGroundEKF::BallEKF::lastUpdated() const {
    return lastUpdateTime;
}

Eigen::Vector4d CameraBallGroundEKF::BallEKF::state() const {
    return X;
}

Eigen::Matrix4d CameraBallGroundEKF::BallEKF::covariance() const {
    return P;
}

bool CameraBallGroundEKF::addObservation(const BallObservation &observation) {
    bool accepted = acceptObservation(observation);
    if (accepted) {
        lastFrameObservations.push_back(observation);
    }
    return accepted;
}

bool CameraBallGroundEKF::processFrame() {
    bool removeFilter = false;
    if (lastFrameObservations.empty()) {
        removeFilter = updateBallNotSeen(ekf.lastUpdated());
    } else if (lastFrameObservations.size() == 1) {
        update(lastFrameObservations.at(0));
    } else {
        //more than one observation. There are a few cases this can happen:
        //1: The ball is seen as two separate balls. This happens occasionally.
        //2: A robot on the frame close to the ball was also detected as a ball.
        //3: (unlikely): there is more than one ball on the field

        //Sort observations by distance to predicted pos
        Eigen::Vector2d predictedPos = ekf.getPosition();
        std::sort(lastFrameObservations.begin(), lastFrameObservations.end(),
                  [predictedPos](const BallObservation &first, const BallObservation &second) {
                      return (first.position - predictedPos).squaredNorm() <
                             (second.position - predictedPos).squaredNorm();
                  });

        //Take the one that's closest and use it
        //If it's the ball we merge it by area with any other detections that are close enough
        Eigen::Vector2d bestDetectionPos = lastFrameObservations.front().position;
        std::vector<BallObservation> closeToBest;
        for(const auto& observation : lastFrameObservations){
            if((observation.position-bestDetectionPos).norm()<0.04){ //TODO: magic constant for merging two balls
                closeToBest.emplace_back(observation);
            }
        }
        BallObservation best = closeToBest[0];
        if(closeToBest.size()>1){
            best = mergeBallObservationsByArea(closeToBest);
        }
        //TODO: what to do with discarded detections? Maybe make acceptance/nonacceptance a different call hierarchy
        //Possibly objects that are 'accepted' are now not used as new object filters
        update(best);
    }
    lastFrameObservations.clear();
    return removeFilter;
}
//TODO: move to CollisionChecker file
std::optional<CollisionChecker::Collision> CameraBallGroundEKF::getFirstCollision(const BallTrajectorySegment& segment, const GeometryData& geometryData, const std::vector<RobotTrajectorySegment>& robotTrajectories){
    std::optional<CollisionChecker::RobotCollisionPreliminaryResult> firstRobotCollision;
    for(const auto& trajectory : robotTrajectories){
        if(auto col = CollisionChecker::checkRobotConstVel(segment,trajectory)){
            if(!firstRobotCollision || col->dt <=  firstRobotCollision->dt){
                firstRobotCollision = col;
            }
        }
    }
    std::optional<CollisionChecker::Collision> robotCollision;
    if(firstRobotCollision){
        robotCollision = CollisionChecker::robotCollideAndReflect(firstRobotCollision.value(),segment);
    }
    std::optional<CollisionChecker::CollisionPreliminaryResult> firstFieldCollision;
    if(auto col = CollisionChecker::getFieldGoalWallCollision(segment,geometryData)){
        firstFieldCollision = col;
    }
    if(auto collision = CollisionChecker::getFieldOutsideWallCollision(segment, geometryData)){
        if(!firstFieldCollision || collision->distanceFraction < firstFieldCollision->distanceFraction){
            firstFieldCollision = collision;
        }
    }
    std::optional<CollisionChecker::Collision> fieldCollision;
    if(firstFieldCollision){
        fieldCollision = CollisionChecker::fieldCollideAndReflect(segment,firstFieldCollision.value());
    }
    if(fieldCollision && robotCollision){
        bool robotFirst = robotCollision->dt <= fieldCollision->dt;
        return robotFirst ? robotCollision.value() : fieldCollision.value();
    }else if(fieldCollision){
        return fieldCollision.value();
    }else if(robotCollision){
        return robotCollision.value();
    }
    return std::nullopt;
}