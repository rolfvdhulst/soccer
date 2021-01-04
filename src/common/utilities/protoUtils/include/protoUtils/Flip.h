//
// Created by rolf on 15-03-20.
//

#ifndef SOCCER_FLIP_H
#define SOCCER_FLIP_H

#include <protobuf/World.pb.h>
#include <protobuf/messages_robocup_ssl_detection.pb.h>
#include <protobuf/messages_robocup_ssl_geometry.pb.h>
#include <math/geometry/LineSegment.h>
#include <math/geometry/Rectangle.h>
#include <math/geometry/Vector2.h>
#include <protobuf/GameState.pb.h>

void flipAndSwap(double &a, double &b);
void flipAndSwap(Vector2 &a, Vector2 &b);
void flipAndSwap(LineSegment &a, LineSegment &b);
void flipAndSwap(Rectangle &a, Rectangle &b);
void flip(proto::World& world);
void flip(proto::SSL_GeometryData& geometryData);
void flip(proto::SSL_GeometryFieldSize* field);
void flip(proto::SSL_DetectionFrame* detectionFrame);
void flip(proto::SSL_DetectionRobot * detectionRobot);
void flip(proto::SSL_DetectionBall * detectionBall);
void flip(proto::GameState& gameState);
void flip(proto::WorldVirtualBall * virtualBall);
#endif  // SOCCER_FLIP_H
