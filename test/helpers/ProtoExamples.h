//
// Created by rolf on 16-03-20.
//

#ifndef SOCCER_PROTOEXAMPLES_H
#define SOCCER_PROTOEXAMPLES_H
#include <protobuf/messages_robocup_ssl_detection.pb.h>
#include <protobuf/messages_robocup_ssl_geometry.pb.h>
//** camera helpers
void addLines(proto::SSL_GeometryFieldSize &field, int fieldLength, int fieldWidth, int defenseWidth, int lineWidth);
void addLinesSwapped(proto::SSL_GeometryFieldSize &field, int fieldLength, int fieldWidth, int defenseWidth, int lineWidth);

proto::SSL_GeometryFieldSize DivisionAField();
proto::SSL_GeometryFieldSize DivisionAFieldSwapped();
proto::SSL_GeometryCameraCalibration RoboCup2019Cam0();
proto::SSL_DetectionRobot cam0Detection();
proto::SSL_GeometryCameraCalibration RoboCup2019Cam1();
proto::SSL_DetectionRobot cam1Detection();
proto::SSL_GeometryCameraCalibration RoboCup2018Cam7();
proto::SSL_DetectionRobot cam7Detection();


#endif  // SOCCER_PROTOEXAMPLES_H
