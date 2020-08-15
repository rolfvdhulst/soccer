//
// Created by rolf on 10-08-20.
//

#ifndef SOCCER_CAMERABALLGROUNDEKF_H
#define SOCCER_CAMERABALLGROUNDEKF_H

#include "CameraObjectFilter.h"
#include <vision/BallObservation.h>
#include <vision/FilteredBall.h>
#include <math/filters/KalmanFilter.h>

class CameraBallGroundEKF : public CameraObjectFilter {
public:
    // TODO: add documentation
    explicit CameraBallGroundEKF(const BallObservation &observation,
                                 Eigen::Vector2d velocityEstimate = Eigen::Vector2d::Zero());

    /**
     * Outputs the current filter state in proto format.
     * @return The Proto message associated with the state of the filter
     */
    [[nodiscard]] FilteredBall getEstimate(const Time &time, bool writeUncertainties = false) const;

    [[nodiscard]] bool justUpdated() const;

    /**
 * Predicts the state of the robot based on past observations.
 * Note this is a permanent update so there is no going back after this is called.
 * @param time The time until we wish to have a prediction of where the robot will be
 */
    void predict(Time time);

    [[nodiscard]] bool acceptObservation(const BallObservation &observation) const;

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

    void registerLogFile(const Eigen::Vector2d &observation);

    void writeLogFile(const Eigen::Vector2d &observation);

private:
    struct BallEKF {
        BallEKF(Eigen::Vector4d initialState,Eigen::Matrix4d initialCovariance,
                double modelError, double measurementError, const Time& timeStamp);
        double modelError;
        Time lastUpdateTime;
        Eigen::Vector4d X;
        Eigen::Matrix4d P;

        Eigen::Matrix4d F;    // Forward model/state update matrix
        Eigen::Matrix<double, 2, 4> H;   // Observation model/ states how we can interpret observation as our state
        Eigen::Matrix4d Q;    // Covariance of the process noise. (Amount of "Random Forces" we can expect in the process)
        Eigen::Matrix2d R;  // Observation Noise Covariance. Keeps track of how noisy the observations are.

        Eigen::Vector2d y; //Innovation. Not strictly necessary to store but often used to measure performance
        void predict(const Time& predictionTime);
        void update(const Eigen::Vector2d& observation);
    private:
        void predictAndSetJacobian(double dt);
        void setProcessNoise(double dt);

    };
    bool lastCycleWasUpdate = true; //The first message (initialization) counts as an update
};


#endif //SOCCER_CAMERABALLGROUNDEKF_H