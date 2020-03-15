//
// Created by rolf on 15-03-20.
//

#ifndef SOCCER_CAMERAMAP_H
#define SOCCER_CAMERAMAP_H
#include <map>
#include "Camera.h"
class CameraMap {
   public:
    void addCamera(const Camera& cam);
    [[nodiscard]] std::optional<const Camera> operator[](int id) const;
    [[nodiscard]] std::optional<const Camera> getCamera(int id) const;

   private:
    std::map<int, Camera> map;
};

#endif  // SOCCER_CAMERAMAP_H
