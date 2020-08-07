//
// Created by rolf on 05-11-19.
//

#ifndef RTT_ROBOTFILTER_H
#define RTT_ROBOTFILTER_H

#include <protobuf/messages_robocup_ssl_detection.pb.h>
#include <math/filters/PosVelFilter2D.h>
#include <vision/RobotObservation.h>
#include <vision/FilteredRobot.h>
#include "CameraObjectFilter.h"
#include "RobotOrientationFilter.h"


/**
 * A class that can filter robots and predict where they will be based on observations.
 * @author Rolf
 * @date 5 November 2019
 */
class CameraRobotFilter : public CameraObjectFilter {
   public:
    /**
     * Construct a RobotFilter.
     * @param detectionRobot Initial observation of the robot we start our filter with.
     * @param detectTime Point in time we start the filter at.
     */
    explicit CameraRobotFilter(const RobotObservation& observation, bool botIsBlue, Eigen::Vector3d velocityEstimate = Eigen::Vector3d::Zero());
    /**
     * Predicts the state of the robot based on past observations.
     * Note this is a permanent update so there is no going back after this is called.
     * @param time The time until we wish to have a prediction of where the robot will be
     */
    void predict(Time time);

    [[nodiscard]] bool acceptObservation(const RobotObservation& observation) const;
    /**
     * Updates the Filter until the specified time, applying observations of the robot and predicting the state along the way.
     * @param time Time until which we want to update.
     * @param doLastPredict In the very last step after applying all the observations, we can choose to not do the last
     * prediction if we do not immediately want to read the filter's data.
     */
    bool update(const RobotObservation& observation);

    [[nodiscard]] bool justUpdated() const;
    /**
     * Updates the filter with the information that we did NOT see the robot on the frame at time t, when it was there at some previous point in time
     * returns true if this filter can be removed (e.g. the health is zero)
     */
    bool updateRobotNotSeen(const Time& time);

    /**
     * Outputs the current filter state in proto format.
     * @return The Proto message associated with the state of the filter
     */
    [[nodiscard]] FilteredRobot getEstimate(const Time& time, bool writeUncertainties = false) const;

    [[nodiscard]] Eigen::Vector3d getVelocity(const Time& time) const;

    void registerLogFile(const Eigen::Vector2d& observation, double observedAngle);
    void writeLogFile(const Eigen::Vector2d& observation, double observedAngle);
   private:
    PosVelFilter2D positionFilter;
    RobotOrientationFilter angleFilter;
    bool lastCycleWasUpdate = true; //The first message (initialization) counts as an update
    int botId;
    int cameraID;
    bool botIsBlue;
    int orientationFilterUniqueId = 0;
};

#endif  // RTT_ROBOTFILTER_H
