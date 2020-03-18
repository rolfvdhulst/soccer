//
// Created by rolf on 15-03-20.
//

#include "CameraMap.h"
void CameraMap::addCamera(const Camera& cam) { map.insert({cam.getID(), cam}); }
std::optional<const Camera> CameraMap::getCamera(int id) const {
    auto cam = map.find(id);
    if (cam != map.end()) {
        return cam->second;
    }
    return std::nullopt;
}
std::optional<const Camera> CameraMap::operator[](int id) const { return getCamera(id); }
void CameraMap::clear() {
    map.clear();
}
bool CameraMap::hasCamera(int id) {
    std::optional<Camera> cam =getCamera(id);
    if (cam){
        return true;
    }
    return false;
}
