//
// Created by rolf on 15-03-20.
//

#ifndef SOCCER_GEOMETRYDATA_H
#define SOCCER_GEOMETRYDATA_H

#include "CameraMap.h"
#include "FieldState.h"
struct GeometryData {
    FieldState field;
    CameraMap cameras;
};
#endif  // SOCCER_GEOMETRYDATA_H
