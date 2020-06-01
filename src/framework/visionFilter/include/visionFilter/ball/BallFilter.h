//
// Created by rolf on 17-11-19.
//

#ifndef RTT_BALLFILTER_H
#define RTT_BALLFILTER_H

#include <protobuf/messages_robocup_ssl_detection.pb.h>
#include <math/filters/PosVelFilter2D.h>
#include <vision/BallObservation.h>
#include <vision/FilteredBall.h>
#include "ObjectFilter.h"

class BallFilter : public ObjectFilter {
   public:
    // TODO: add documentation
    explicit BallFilter(const BallObservation& observation);
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
    void predict(Time time);
    /**
     * Updates the Filter until the specified time, applying observations
     * @param time Time until which we want to update.
     * @param doLastPredict In the very last step after applying all the observations, we can choose to not do the last
     * prediction if we do not immediately want to read the filter's data.
     */
    bool update(const BallObservation& observation);
    /**
     * Updates the filter with the information that we did NOT see the robot on the frame at time t, when it was there at some previous point in time
     */
    void updateBallNotSeen(const Time& time);

   private:
    PosVelFilter2D positionFilter;
    bool lastCycleWasUpdate = true; //The first message (initialization) counts as an update

};

#endif  // RTT_BALLFILTER_H
