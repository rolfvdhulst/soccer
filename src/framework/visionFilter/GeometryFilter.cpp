//
// Created by rolf on 22-03-20.
//

#include "GeometryFilter.h"
bool GeometryFilter::process(const proto::SSL_GeometryData &geometryData) {
    //We serialize the data to string and check if it is the same as the last one.
    //If so, there's no point in updating the geometry and we just return.
    lastGeomTime = Time::now();
    std::string geomString = geometryData.SerializeAsString();
    if (geomString == lastGeometryString){
        return false;
    }
    // New message:
    // Add/overwrite any new camera info
    for (const auto& cameraCalib : geometryData.calib()) {
        cameras[cameraCalib.camera_id()] = cameraCalib;
    }
    combinedGeometry.clear_calib();
    for (const auto& cam : cameras){
        auto calibration = combinedGeometry.add_calib();
        calibration->CopyFrom(cam.second);
    }
    if (geometryData.has_field()){
        combinedGeometry.mutable_field()->CopyFrom(geometryData.field());
    }
    return true;
}

Time GeometryFilter::lastUpdateTime() const {
    return lastGeomTime;
}
const proto::SSL_GeometryData& GeometryFilter::getGeometry() {
    return combinedGeometry;
}
