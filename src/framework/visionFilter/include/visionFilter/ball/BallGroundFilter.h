//
// Created by rolf on 10-08-20.
//

#ifndef SOCCER_CAMERABALLGROUNDEKF_H
#define SOCCER_CAMERABALLGROUNDEKF_H

#include "CameraObjectFilter.h"
#include <vision/BallObservation.h>
#include <vision/FilteredBall.h>
#include <math/filters/KalmanFilter.h>
#include <field/GeometryData.h>
#include "RobotTrajectorySegment.h"
#include "ball/collision/CollisionChecker.h"
#include "ball/BallGroundEKF.h"

class BallGroundFilter : public CameraObjectFilter {
public:
    explicit BallGroundFilter(const BallObservation &observation,
                              Eigen::Vector2d velocityEstimate = Eigen::Vector2d::Zero());

    /**
     * Outputs the current filter state in proto format.
     * @return The Proto message associated with the state of the filter
     */
    [[nodiscard]] FilteredBall getEstimate(const Time &time, bool writeUncertainties = false) const;

    struct PredictedBall{
        Eigen::Vector2d position;
        Eigen::Vector2d velocity;
        Time time;
        std::vector<CollisionChecker::Collision> collisions;
    };

    struct PredictedBalls{
        std::vector<PredictedBall> balls;
    };
    struct ObservationPredictionPair{
        PredictedBall prediction;
        std::optional<BallObservation> observation;
        int objectID;
    };
    [[nodiscard]] PredictedBalls predict(Time time, const GeometryData& geometryData, const std::vector<RobotTrajectorySegment>& robotTrajectories) const;

    [[nodiscard]] Eigen::Vector2d getVelocity(const Time &time) const;

    /**
     * Updates the Filter until the specified time, applying observations
     * @param time Time until which we want to update.
     * @param doLastPredict In the very last step after applying all the observations, we can choose to not do the last
     * prediction if we do not immediately want to read the filter's data.
     */
    void update(const BallObservation &observation);

    /**
     * Updates the filter with the information that we did NOT see the robot on the frame at time t, when it was there at some previous point in time
     */
    bool updateBallNotSeen(const Time &time);

//    void registerLogFile(const Eigen::Vector2d &observation);
//
//    void writeLogFile(const Eigen::Vector2d &observation);

    bool processDetections(const ObservationPredictionPair& opPair);

private:
    BallGroundEKF ekf;
    void predictFilter(const PredictedBall& prediction);

};


#endif //SOCCER_CAMERABALLGROUNDEKF_H
