#ifndef SOCCER_WORLDFILTER_H
#define SOCCER_WORLDFILTER_H

#include <protobuf/World.pb.h>
#include <protobuf/WorldRobot.pb.h>
#include <protobuf/messages_robocup_ssl_geometry.pb.h>
#include <field/CameraMap.h>
#include "BallFilter.h"
#include "RobotFilter.h"


/**
 * @author Rolf van der Hulst
 * @date November 2019
 * @brief class that tracks a world. The function of this class is to determine when to create and delete filters,
 * and to pass the relevant incoming information to the relevant filter(s)
 */
class WorldFilter {
   public:
    WorldFilter();
    /** Add a frame to the WorldFilter. This will be forwarded to the relevant filters (ball/robot)
     *  Or they will be created if they do not exist yet. Note this does NOT call the Kalman update/predict equations and thus
     *  only puts the data in the right spot. Worldfilter itself manages the messages based on their timestamps,
     *  so in between update/predict calls the order in which you supply messages does not matter.
     *  @param msg Frame to be given to the filter
     */
    void addFrame(const proto::SSL_DetectionFrame &msg);
    /**
     * Get the state estimation of the world as a proto message. This also calls update(), ensuring that data is sent
     * is as up to date as possible.
     * @param time The time at which you want an estimation of the world state.
     * This should not be much more late than the latest message or else you will get very unphysical results.
     * @return Proto message containing the entire world state.
     */
    proto::World getWorld(Time time);
    /**
     * Updates the world filter's state estimation until a certain time.
     * @param time Time to update to.
     * @param doLastPredict If set to true, all filters  predict/extrapolate the positions of all objects
     * from the last time they were seen until the time specified.
     * You should always set this to true if you plan on using data immediately.
     */
    void update(Time time, bool doLastPredict);
    /**
     * Updates the cameras which the worldFilter uses for calculations.
     * @param geometry to grab the cameras from
     */
    void updateCameras(const proto::SSL_GeometryData& geometry);

   private:
    typedef std::map<int, std::vector<std::unique_ptr<RobotFilter>>> robotMap;
    static const std::unique_ptr<RobotFilter> &bestFilter(const std::vector<std::unique_ptr<RobotFilter>> &filters);
    static const std::unique_ptr<BallFilter> &bestFilter(const std::vector<std::unique_ptr<BallFilter>> &filters);

    robotMap blueBots;
    robotMap yellowBots;
    CameraMap cameras;
    std::vector<std::unique_ptr<BallFilter>> balls;
    static void updateRobots(robotMap &robots, Time time, bool doLastPredict, Time removeFilterTime);
    static void handleRobots(robotMap &robots, const google::protobuf::RepeatedPtrField<proto::SSL_DetectionRobot> &observations, double filterGrabDistance, const Time& timeCapture,
                             const Time& timeSent, uint cameraID);
    void handleBall(const google::protobuf::RepeatedPtrField<proto::SSL_DetectionBall> &observations, double filterGrabDistance, const Time& timeCapture, const Time& timeSent, uint cameraID);
    std::vector<BallObservation> ballObservations;
    void updateBalls(Time time, bool doLastPredict, const Time removeFilterTime);
};

#endif  // SOCCER_WORLDFILTER_H