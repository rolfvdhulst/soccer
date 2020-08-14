//
// Created by rolf on 05-08-20.
//

#ifndef SOCCER_BALLFILTER_H
#define SOCCER_BALLFILTER_H

#include "ObjectFilter.h"
#include <vision/BallObservation.h>
#include "ball/CameraBallFilter_v2.h"

class BallFilter : public ObjectFilter {
public:
    explicit BallFilter(const BallObservation &observation);
    bool processDetection(const BallObservation& observation);
    void predictCam(const int& cameraID, const Time& untilTime, const GeometryData& geometryData);

    FilteredBall mergeBalls(const Time& time) const;
    /**
     * Checks if a camera has not seen, and if so, processes the fact that an object was not seen by a camera frame taken at time
     * @param cameraID
     * @param untilTime
     * @return true if this filter can be removed (e.g. is empty), false otherwise
     */
    bool processNotSeen(const int& cameraID, const Time& time);

    double getHealth() const;

private:
    std::map<int, CameraBallFilter_v2> cameraFilters;
};


#endif //SOCCER_BALLFILTER_H
