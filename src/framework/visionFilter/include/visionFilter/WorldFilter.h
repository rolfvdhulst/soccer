#ifndef SOCCER_WORLDFILTER_H
#define SOCCER_WORLDFILTER_H

#include <protobuf/World.pb.h>
#include <protobuf/messages_robocup_ssl_geometry.pb.h>
#include <field/GeometryData.h>
#include "RobotFilter.h"
#include "BallFilter.h"
#include <vision/DetectionFrame.h>

/**
 * @author Rolf van der Hulst
 * @date November 2019
 * @brief class that tracks a world. The function of this class is to determine when to create and delete filters,
 * and to pass the relevant incoming information to the relevant filter(s)
 */
class WorldFilter {
   public:
    WorldFilter() = default;

    void process(const std::vector<proto::SSL_DetectionFrame> &frames);

    [[nodiscard]] proto::World getWorld(const Time& time) const;

    /**
     * Updates the cameras which the worldFilter uses for calculations, and the field geometry which is used for detecting
     * wall collisions.
     * @param geometry to grab the cameras from
     */
    void updateGeometry(const proto::SSL_GeometryData& geometry);

   private:
    typedef std::map<int, std::vector<RobotFilter>> robotMap;
    robotMap blue;
    robotMap yellow;
    std::vector<BallFilter> balls; // A list containing all filters which are tracking balls
    GeometryData geometryData;
    const int MAX_ROBOTFILTERS = 5;
    const int MAX_BALLFILTERS = 8;

    void processRobots(const DetectionFrame& frame, bool blueBots);
    void processBalls(const DetectionFrame& frame);

    static void predictRobots(const DetectionFrame &frame, robotMap &robots);
    void updateRobots(bool blueBots, robotMap &robots, const std::vector<RobotObservation> &detectedRobots);
    static void updateRobotsNotSeen(const DetectionFrame &frame,robotMap &robots);
};

#endif  // SOCCER_WORLDFILTER_H
