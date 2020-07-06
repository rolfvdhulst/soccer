//
// Created by kjhertenberg on 13-5-19.
//

#include "WorldFilter.h"
#include <protobuf/messages_robocup_ssl_detection.pb.h>
#include <protobuf/messages_robocup_ssl_geometry.pb.h>
#include <field/Camera.h>
#include <vision/DetectionFrame.h>

WorldFilter::WorldFilter() {
}

// if we get a new frame we update our observations
void WorldFilter::addFrame(const proto::SSL_DetectionFrame &msg) {
    DetectionFrame frame(msg);
    if(cameraFilters.find(frame.cameraID) != cameraFilters.end()){
        CameraFilter& filter = cameraFilters.at(frame.cameraID);
        filter.process(frame);
    }else{
        //create new camera filter
        if(cameras.hasCamera(frame.cameraID)){
            cameraFilters.insert(std::pair<int,CameraFilter>(frame.cameraID,CameraFilter(frame)));
        }else{
            cameraFilters.insert(std::pair<int,CameraFilter>(frame.cameraID,CameraFilter(frame)));
        }
    }
}

void WorldFilter::updateCameras(const proto::SSL_GeometryData &geometry) {
    cameras.clear();
    for (const auto& cam : geometry.calib()){
        cameras.addCamera(Camera(cam));
    }
    for(auto& filter : cameraFilters){
        if(cameras.hasCamera(filter.first)){
            filter.second.updateCalibration(*cameras.getCamera(filter.first));
        }
    }
}

proto::World WorldFilter::getWorld(const Time &time) const {
    proto::World world;
    world.set_time(time.asNanoSeconds());
    double currentHealth = -1;
    std::optional<FilteredBall> bestBall = std::nullopt;
    for(const auto& filter : cameraFilters){
        std::optional<FilteredBall> filteredBall = filter.second.getBestBall(time);
        if(filteredBall && filteredBall->health > currentHealth){
            currentHealth = filteredBall->health;
            bestBall = filteredBall;
        }
    }
    if(bestBall){
        world.mutable_ball()->CopyFrom(bestBall->asWorldBall());
    }
    for (int id = 0; id < 16; ++id) {
        currentHealth = -1;
        std::optional<FilteredRobot> robot;
        for(const auto& filter : cameraFilters){
            std::optional<FilteredRobot> filteredRobot = filter.second.getBestRobot(time,id,true);
            if(filteredRobot && filteredRobot->health > currentHealth){
                currentHealth = filteredRobot->health;
                robot = filteredRobot;
            }
        }
        if(robot){
            world.add_blue()->CopyFrom(robot->asWorldRobot());
        }
    }

    for (int id = 0; id < 16; ++id) {
        currentHealth = -1;
        std::optional<FilteredRobot> robot;
        for(const auto& filter : cameraFilters){
            std::optional<FilteredRobot> filteredRobot = filter.second.getBestRobot(time,id,false);
            if(filteredRobot && filteredRobot->health > currentHealth){
                currentHealth = filteredRobot->health;
                robot = filteredRobot;
            }
        }
        if(robot){
            world.add_yellow()->CopyFrom(robot->asWorldRobot());
        }
    }
    return world;
}
