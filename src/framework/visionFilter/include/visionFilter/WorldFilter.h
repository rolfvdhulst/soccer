#ifndef SOCCER_WORLDFILTER_H
#define SOCCER_WORLDFILTER_H

#include <protobuf/World.pb.h>
#include <protobuf/messages_robocup_ssl_geometry.pb.h>
#include <field/CameraMap.h>
#include "CameraFilter.h"


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
    [[nodiscard]] proto::World getWorld(const Time& time) const;

    /**
     * Updates the cameras which the worldFilter uses for calculations.
     * @param geometry to grab the cameras from
     */
    void updateCameras(const proto::SSL_GeometryData& geometry);

   private:

    CameraMap cameras;
    std::map<int,CameraFilter> cameraFilters;
};

#endif  // SOCCER_WORLDFILTER_H
