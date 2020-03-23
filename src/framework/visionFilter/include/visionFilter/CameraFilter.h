//
// Created by rolf on 15-12-19.
//

#ifndef RTT_CAMERAFILTER_H
#define RTT_CAMERAFILTER_H

#include <core/Time.h>
/*
 * This class relies on it's superclasses to set lastUpdateTime and framecount correctly!
 * lastMainUpdate is updated correctly if switchCamera is called for every update, as it should be
 */
class CameraFilter {
   private:
    int frameCount = 0;

   protected:
    Time lastUpdateTime;
    Time lastMainUpdateTime;
    int mainCamera = -1;

    /**
     * Given a camera detection at time, decides whether we should switch our main camera or not.
     * This is implemented as a check: if we receive a frame from another camera,
     * and we have not received a frame from our main camera for some time, we switch our main camera to camera ID passed to the function.
     * @return true if we switched our main camera
     */
    bool switchCamera(int camera, Time time);

   public:
    explicit CameraFilter(Time observationTime, int camera);
    /**
     * The time of the last observation which was processed by the filter
     * @return The time at which the filter was last updated
     */
    [[nodiscard]] Time getLastUpdateTime() const;
    /**
     * The amount of observations the filter has processed
     * @return The amount of observations the filter processed
     */
    [[nodiscard]] int frames() const;
};

#endif  // RTT_CAMERAFILTER_H
