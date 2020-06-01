//
// Created by rolf on 15-12-19.
//

#ifndef RTT_CAMERAFILTER_H
#define RTT_CAMERAFILTER_H

#include <core/Time.h>
#include "RobotFilter.h"
#include "ball/BallFilter.h"
#include <protobuf/World.pb.h>
#include <field/Camera.h>
#include <vision/DetectionFrame.h>
#include <protobuf/RobotInfo.h>

class CameraFilter {
private:
    typedef std::map<int, std::vector<RobotFilter>> robotMap;
    robotMap blue;
    robotMap yellow;
    std::vector<BallFilter> balls; // A list containing all Filters that are tracking balls
    const int MAX_BALLFILTERS = 10;
    //the following we might want to pass as parameters instead of saving them here
    std::optional<Camera> camera; //Position and direction data of the camera
    proto::TeamRobotInfo teamInfo;
    Time lastDetectionFrameTime;
public:
    explicit CameraFilter(const DetectionFrame& frame);
    explicit CameraFilter(const DetectionFrame& frame,const Camera& calibration);
    void updateCalibration(const Camera &camera);
    void updateRobotInfo(const proto::TeamRobotInfo& robotInfo);
    void process(const DetectionFrame& frame);
    void processRobots(const DetectionFrame &frame, bool blueBots);
    void processBalls(const DetectionFrame &frame);
    std::optional<FilteredBall> getBestBall(const Time& time) const;
    std::optional<FilteredRobot> getBestRobot(const Time& time, int id, bool blueBots) const;
    std::vector<FilteredBall> getHealthyBalls(const Time& time) const;
    std::vector<FilteredRobot> getHealthyRobots(const Time& time, int id, bool blueBots) const;
};

#endif  // RTT_CAMERAFILTER_H
