//
// Created by rolf on 20-01-21.
//

#ifndef SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_COLLISION_COLLISIONCLASSIFIER_H_
#define SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_COLLISION_COLLISIONCLASSIFIER_H_

#include <vision/BallObservation.h>
#include <containers/circular_buffer.h>
#include "CollisionChecker.h"

enum class CollisionState{
  COLLIDED,
  UNCERTAIN,
  NO_COLLISION,
  UNOBSERVED
};

class CollisionClassifier {
 public:
  void addObservation(const BallObservation& observation);
  CollisionState detectRobotFrontCollision(const CollisionChecker::Collision& robot_front_collision);
 private:
  circular_buffer<BallObservation,3> buffer;
};

#endif //SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_COLLISION_COLLISIONCLASSIFIER_H_
