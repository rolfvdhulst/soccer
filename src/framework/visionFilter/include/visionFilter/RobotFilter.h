//
// Created by rolf on 05-08-20.
//

#ifndef SOCCER_ROBOTFILTER_H
#define SOCCER_ROBOTFILTER_H

#include "ObjectFilter.h"
#include "CameraRobotFilter.h"
#include <vision/RobotObservation.h>
class RobotFilter : public ObjectFilter {
public:
    explicit RobotFilter(const RobotObservation &observation, bool botIsBlue);
    bool processDetection(const RobotObservation&observation);
    void predictCam(const int& cameraID, const Time& untilTime);

    /**
     * Checks if a camera has not seen, and if so, processes the fact that an object was not seen by a camera frame taken at time
     * @param cameraID
     * @param untilTime
     * @return true if this filter can be removed (e.g. is empty), false otherwise
     */
    bool processNotSeen(const int& cameraID, const Time& time);

    double getHealth() const;
    FilteredRobot mergeRobots(const Time& time) const;
private:
    int robotID;
    bool isBlue;
    std::map<int,CameraRobotFilter> cameraFilters;
};


#endif //SOCCER_ROBOTFILTER_H
