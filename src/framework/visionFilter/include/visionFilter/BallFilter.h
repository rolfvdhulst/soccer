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

class BallPredictions {
public:
    CameraBallGroundEKF::PredictedBalls balls;
    bool hadRequestedCamera;
    int objectID;
};

class BallFilter : public ObjectFilter {
public:
    explicit BallFilter(const BallObservation &observation);

    [[nodiscard]] BallPredictions predictCam(int cameraID, const Time &untilTime, const GeometryData &geometryData,
                                             const std::vector<RobotTrajectorySegment> &robotTrajectorySegments) const;

    bool processFrame(int cameraID, Time time);

    [[nodiscard]] FilteredBall mergeBalls(const Time &time) const;

    [[nodiscard]] double getHealth() const;

    [[nodiscard]] BallObservation lastDetection() const;

private:
    std::map<int, CameraBallGroundEKF> cameraFilters;
    circular_buffer<BallObservation, 300> acceptedBalls;
};


#endif //SOCCER_BALLFILTER_H
