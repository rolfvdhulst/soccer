//
// Created by rolf on 22-03-20.
//

#include "GeometryFilter.h"
void GeometryFilter::process(const proto::SSL_GeometryData &geometryData) {
    //We serialize the data to string and check if it is the same as the last one.
    //If so, there's no point in updating the geometryFilter
    lastGeomTime = Time::now();
    std::string geomString = geometryData.SerializeAsString();
    if (geomString == lastGeometryString){
        return;
    }
    // New message:
    geomUpdates++;
    // Add/overwrite any new camera info
    for (const auto& cameraCalib : geometryData.calib()) {
        cameraMap.addCamera(Camera(cameraCalib));
    }
    if (geometryData.has_field()){
        //Warning, this constructor/data type is quite complex because the SSL does not do consistent standards that well.
        field = FieldState(geometryData.field());
    }
}
const FieldState &GeometryFilter::getField() const {
    return field;
}
const CameraMap &GeometryFilter::getCameras() const {
    return cameraMap;
}
Time GeometryFilter::lastUpdateTime() const {
    return lastGeomTime;
}
bool GeometryFilter::receivedMessage() const {
    return geomUpdates>0;
}
