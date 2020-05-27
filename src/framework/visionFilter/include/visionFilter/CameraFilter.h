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
#include <containers/circular_buffer.h>
class CameraFilter {
private:
    typedef std::map<int, std::vector<RobotFilter>> robotMap;
    robotMap blue;
    robotMap yellow;
    std::vector<BallFilter> balls; // A list containing all Filters that are tracking balls
    std::optional<Camera> camera; //Position and direction data of the camera
    circular_buffer<BallObservation,100> lastDetections;
public:
};

#endif  // RTT_CAMERAFILTER_H
