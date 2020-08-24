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

class CameraBallGroundEKF : public CameraObjectFilter {
public:
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
    void predict(Time time, const GeometryData& geometryData);

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

    bool addObservation(const BallObservation &observation);

    bool processFrame();

private:
    struct BallEKF {
        BallEKF() = default;
        BallEKF(Eigen::Vector4d initialState,Eigen::Matrix4d initialCovariance,
                double modelError, double measurementError, const Time& timeStamp);
    private:
        double modelError;
        double acc = - 0.3;
        Time lastUpdateTime;
        Eigen::Vector4d X;
        Eigen::Matrix4d P;

        Eigen::Matrix4d F;    // Forward model/state update matrix
        Eigen::Matrix<double, 2, 4> H;   // Observation model/ states how we can interpret observation as our state
        Eigen::Matrix4d Q;    // Covariance of the process noise. (Amount of "Random Forces" we can expect in the process)
        Eigen::Matrix2d R;  // Observation Noise Covariance. Keeps track of how noisy the observations are.

        Eigen::Vector2d y; //Innovation. Not strictly necessary to store but often used to measure performance

        void setProccessNoise(double dt);
    public:

        void predict(const Time& predictionTime);
        void update(const Eigen::Vector2d& observation);
        //TODO: use the below functions in the appropriate places
        [[nodiscard]] Eigen::Vector2d getPosition() const;
        [[nodiscard]] Eigen::Vector2d getPositionEstimate(const Time& time) const;
        [[nodiscard]] Eigen::Vector4d getStateEstimate(const Time& time) const;
        [[nodiscard]] Eigen::Vector4d getStateEstimate(double dt) const;
        [[nodiscard]] Eigen::Vector2d getVelocity() const;
        [[nodiscard]] Eigen::Vector2d getVelocityEstimate(const Time& time) const;
        void setVelocity(const Eigen::Vector2d& velocity);
        void addUncertainty(double posUncertainty, double velUncertainty);
        //TODO: make these time-based?
        [[nodiscard]] Eigen::Vector2d getVelocityUncertainty() const;
        [[nodiscard]] Eigen::Vector2d getPositionUncertainty() const;
        [[nodiscard]] double getAcc() const;
        void setAcc(double acceleration);

        [[nodiscard]] Eigen::Vector2d innovation() const;
        [[nodiscard]] Time lastUpdated() const;

        [[nodiscard]] Eigen::Vector4d state() const;
        [[nodiscard]] Eigen::Matrix4d covariance() const;
    };
    BallEKF ekf;
    std::vector<BallObservation> lastFrameObservations;
    bool lastCycleWasUpdate = true; //The first message (initialization) counts as an update
};


#endif //SOCCER_CAMERABALLGROUNDEKF_H
