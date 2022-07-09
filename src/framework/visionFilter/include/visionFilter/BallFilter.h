//
// Created by rolf on 05-08-20.
//

#ifndef SOCCER_BALLFILTER_H
#define SOCCER_BALLFILTER_H

#include "ObjectFilter.h"
#include <vision/BallObservation.h>
#include "ball/BallGroundFilter.h"
#include "ball/BallFlyFilter.h"
#include "RobotTrajectorySegment.h"
#include <containers/circular_buffer.h>
#include "ball/kick/detection/KickEventFilter.h"

class BallPredictions {
public:
    BallGroundFilter::PredictedBalls balls;
    bool hadRequestedCamera;
    int objectID;
    KickPredictions kick_predictions;
};

class BallFilter : public ObjectFilter {
public:
    explicit BallFilter(const BallObservation &observation);

    [[nodiscard]] BallPredictions predictCam(int cameraID, const Time &untilTime, const GeometryData &geometryData,
                                             const std::vector<RobotTrajectorySegment> &robotTrajectorySegments) const;

    bool processDetections(const BallGroundFilter::ObservationPredictionPair& detections, int cameraID);

    [[nodiscard]] FilteredBall mergeBalls(const Time &time) const;

    [[nodiscard]] double getHealth() const;

    [[nodiscard]] BallObservation lastDetection() const;

private:
    std::map<int, BallGroundFilter> groundFilters;
    KickEventFilter kick_detector;
    BallFlyFilter fly_filter;
    circular_buffer<BallObservation, 300> acceptedBalls;
};


#endif //SOCCER_BALLFILTER_H
