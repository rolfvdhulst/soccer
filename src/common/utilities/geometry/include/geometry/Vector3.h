//
// Created by rolf on 09-03-20.
//

#ifndef SOCCER_VECTOR3_H
#define SOCCER_VECTOR3_H

#include <Eigen/Eigen>
// We don't want to reinvent the wheel but a consistent definition is nice.
typedef Eigen::Vector3d Vector3;
typedef Eigen::Vector2d Vector2Eigen;
typedef Eigen::Quaterniond Quaternion;
typedef Eigen::Matrix3d Matrix3;

double rayPlaneIntersection(const Vector3& planeOrigin, const Vector3& planeNormal, const Vector3& rayOrigin, const Vector3& rayVector);

#endif  // SOCCER_VECTOR3_H
