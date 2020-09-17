//
// Created by rolf on 05-08-20.
//

#ifndef SOCCER_BALLFILTER_H
#define SOCCER_BALLFILTER_H

#include "ObjectFilter.h"
#include <vision/BallObservation.h>
#include "ball/CameraBallFilter_v2.h"
#include "ball/CameraBallGroundEKF.h"
#include "RobotTrajectorySegment.h"
#include <containers/circular_buffer.h>

class BallFilter : public ObjectFilter {
public:
    explicit BallFilter(const BallObservation &observation);
    void predictCam(int cameraID, const Time &untilTime, const GeometryData& geometryData, const std::vector<RobotTrajectorySegment>& robotTrajectorySegments);
    bool acceptDetection(const BallObservation& observation);
    bool processFrame(int cameraID, Time time);

    FilteredBall mergeBalls(const Time& time) const;

    double getHealth() const;

    BallObservation lastDetection() const;
private:
    std::map<int, CameraBallGroundEKF> cameraFilters;
    circular_buffer<BallObservation,300> acceptedBalls;
};


#endif //SOCCER_BALLFILTER_H
