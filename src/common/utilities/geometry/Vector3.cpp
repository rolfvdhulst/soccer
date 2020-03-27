//
// Created by rolf on 09-03-20.
//

#include "Vector3.h"

double rayPlaneIntersection(const Eigen::Vector3d &planeOrigin, const Eigen::Vector3d &planeNormal, const Eigen::Vector3d &rayOrigin, const Eigen::Vector3d &rayVector) {
    return (-planeNormal).dot(rayOrigin - planeOrigin) / (planeNormal.dot(rayVector));
}