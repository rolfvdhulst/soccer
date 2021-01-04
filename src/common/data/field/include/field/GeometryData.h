//
// Created by rolf on 15-03-20.
//

#ifndef SOCCER_GEOMETRYDATA_H
#define SOCCER_GEOMETRYDATA_H

#include "CameraMap.h"
#include "FieldState.h"
struct GeometryData {
    GeometryData() = default;
    explicit GeometryData(const proto::SSL_GeometryData& protoInfo) :
    field(protoInfo.field()){
        for (const auto& cam : protoInfo.calib()){
            cameras.addCamera(Camera(cam));
        }
    }
    FieldState field;
    CameraMap cameras;
};
#endif  // SOCCER_GEOMETRYDATA_H
