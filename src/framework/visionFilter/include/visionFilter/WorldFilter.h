#ifndef SOCCER_WORLDFILTER_H
#define SOCCER_WORLDFILTER_H

#include <protobuf/World.pb.h>
#include <protobuf/messages_robocup_ssl_geometry.pb.h>
#include <field/GeometryData.h>
#include "RobotFilter.h"
#include "BallFilter.h"
#include <vision/DetectionFrame.h>
#include <protobuf/RobotInfo.pb.h>
#include <world/RobotParameters.h> //TODO: refactor robot relevant things (e.g. RobotID, RobotParameters etc.) to a library and remove world from library again
#include "ball/VirtualBallTracker.h"

/**
 * @author Rolf van der Hulst
 * @date November 2019
 * @brief class that tracks a world. The function of this class is to determine when to create and delete filters,
 * and to pass the relevant incoming information to the relevant filter(s)
 */
class WorldFilter {
   public:
    WorldFilter();

    void process(const std::vector<proto::SSL_DetectionFrame> &frames);

    [[nodiscard]] proto::World getWorld(const Time& time) const;

    /**
     * Updates the cameras which the worldFilter uses for calculations, and the field geometry which is used for detecting
     * wall collisions.
     * @param geometry to grab the cameras from
     */
    void updateGeometry(const proto::SSL_GeometryData& geometry);

    void updateRobotParameters(const proto::TeamRobotInfo& robotInfo); //TODO: link to interface

   private:
    typedef std::map<int, std::vector<RobotFilter>> robotMap;
    robotMap blue;
    robotMap yellow;
    std::vector<BallFilter> balls; // A list containing all filters which are tracking balls
    bool isTrackingVirtualBalls = false;
    VirtualBallTracker virtualBallTracker;
    GeometryData geometryData;
    RobotParameters blueParams;
    RobotParameters yellowParams;
    std::vector<RobotTrajectorySegment> robotTrajectories;
    std::map<int,Time> lastCaptureTimes;

    const int MAX_ROBOTFILTERS = 5;
    const int MAX_BALLFILTERS = 8;

    void processFrame(const DetectionFrame& frame);
    void processRobots(const DetectionFrame& frame, bool blueBots);
    void processBalls(const DetectionFrame& frame, const std::vector<RobotTrajectorySegment>& robotTrajectories);
    void processForVirtualBalls(const DetectionFrame& frame);
    std::vector<FilteredRobot> getHealthiestRobotsMerged(bool blueBots, Time time)  const;
    std::vector<FilteredRobot> oneCameraHealthyRobots(bool blueBots, int camera_id, Time time) const;
    void getPreviousFrameTrajectories(bool isBlue, int cameraID);

    static void predictRobots(const DetectionFrame &frame, robotMap &robots);
    void updateRobots(bool blueBots, robotMap &robots, const std::vector<RobotObservation> &detectedRobots);
    static void updateRobotsNotSeen(const DetectionFrame &frame,robotMap &robots);
};

#endif  // SOCCER_WORLDFILTER_H
