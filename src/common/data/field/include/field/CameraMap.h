//
// Created by rolf on 15-03-20.
//

#ifndef SOCCER_CAMERAMAP_H
#define SOCCER_CAMERAMAP_H
#include <map>
#include "Camera.h"
class CameraMap {
   public:
        /**
         * @brief adds a camera to the map
         * @param cam Camera to add
         */
    void addCamera(const Camera& cam);
    /**
     * Same as getCamera() but familiar syntax is nice.
     * @param id cameraId to get.
     * @return an optional that contains the camera if it exists in the map.
     */
    [[nodiscard]] std::optional<const Camera> operator[](int id) const;
    /**
     * @param id camera to get.
     * @return an optional that contains the camera if it exists in the map.
     */
    [[nodiscard]] std::optional<const Camera> getCamera(int id) const;
    /**
     * @param id
     * @return true if the camera exists in the map
     */
    bool hasCamera(int id);
    /**
     * @brief Clears all camera's from the map
     */
    void clear();

   private:
    std::map<int, Camera> map;
};

#endif  // SOCCER_CAMERAMAP_H
