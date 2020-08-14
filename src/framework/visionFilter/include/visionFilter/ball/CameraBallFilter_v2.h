// Created by rolf on 11-08-20.
//

#ifndef SOCCER_CAMERABALLFILTER_V2_H
#define SOCCER_CAMERABALLFILTER_V2_H

#include <vision/BallObservation.h>
#include <vision/FilteredBall.h>
#include "CameraObjectFilter.h"
#include <math/filters/PosVelFilter2D.h>
#include <field/GeometryData.h>


class CameraBallFilter_v2 : public CameraObjectFilter{
public:
    explicit CameraBallFilter_v2(const BallObservation& observation, Eigen::Vector2d velocityEstimate = Eigen::Vector2d::Zero());
    /**
     * Outputs the current filter state in proto format.
     * @return The Proto message associated with the state of the filter
     */
    [[nodiscard]] FilteredBall getEstimate(const Time& time, bool writeUncertainties = false) const;
    [[nodiscard]] bool justUpdated() const;
    /**
 * Predicts the state of the robot based on past observations.
 * Note this is a permanent update so there is no going back after this is called.
 * @param time The time until we wish to have a prediction of where the robot will be
 */
    void predict(const Time& time, const GeometryData& geometryData);

    [[nodiscard]] bool acceptObservation(const BallObservation& observation) const;

    [[nodiscard]] Eigen::Vector2d getVelocity(const Time& time) const;
    /**
     * Updates the Filter until the specified time, applying observations
     * @param time Time until which we want to update.
     * @param doLastPredict In the very last step after applying all the observations, we can choose to not do the last
     * prediction if we do not immediately want to read the filter's data.
     */
    void update(const BallObservation& observation);
    /**
     * Updates the filter with the information that we did NOT see the robot on the frame at time t, when it was there at some previous point in time
     */
    bool updateBallNotSeen(const Time& time);

    void registerLogFile(const Eigen::Vector2d& observation);
    void writeLogFile(const Eigen::Vector2d& observation);
private:
    PosVelFilter2D positionFilter;
    bool lastCycleWasUpdate = true; //The first message (initialization) counts as an update
};


#endif //SOCCER_CAMERABALLFILTER_V2_H
