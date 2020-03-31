//
// Created by rolf on 15-03-20.
//

#ifndef SOCCER_FLIP_H
#define SOCCER_FLIP_H

#include <protobuf/World.pb.h>
#include <protobuf/messages_robocup_ssl_detection.pb.h>
#include "LineSegment.h"
#include "Rectangle.h"
#include "Vector2.h"

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
#endif  // SOCCER_FLIP_H
