//
// Created by rolf on 05-08-20.
//

#ifndef SOCCER_BALLFILTER_H
#define SOCCER_BALLFILTER_H

#include "ObjectFilter.h"
#include <vision/BallObservation.h>
#include "ball/CameraBallFilter_v2.h"
#include "ball/CameraBallGroundEKF.h"
class BallFilter : public ObjectFilter {
public:
    explicit BallFilter(const BallObservation &observation);
    void predictCam(int cameraID, const Time &untilTime, const GeometryData& geometryData);
    bool acceptDetection(const BallObservation& observation);
    bool processFrame(int cameraID, Time time);

    FilteredBall mergeBalls(const Time& time) const;

    double getHealth() const;

private:
    std::map<int, CameraBallGroundEKF> cameraFilters;
};


#endif //SOCCER_BALLFILTER_H
