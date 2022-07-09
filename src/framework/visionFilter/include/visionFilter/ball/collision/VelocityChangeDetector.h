//
// Created by rolf on 07-01-21.
//

#ifndef SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_COLLISION_VELOCITYCHANGEDETECTOR_H_
#define SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_COLLISION_VELOCITYCHANGEDETECTOR_H_

#include <containers/circular_buffer.h>
#include <vision/BallObservation.h>
#include "CollisionChecker.h"
enum class CollisionState{
  COLLIDED,
  UNCERTAIN,
  NO_COLLISION,
  UNOBSERVED
};


class VelocityChangeDetector {
 public:
  void addObservation(const BallObservation& observation);
  bool detectChange(); //Should only be called ONCE every time an observation is added to the filter!!
  [[nodiscard]] CollisionState estimateCollision(const CollisionChecker::Collision& collision) const;
  Time lastTimeDifference() const;
 private:
  [[nodiscard]] bool detectKick() const;
  circular_buffer<BallObservation,3> buffer;
  bool previousWasKick = false;
};

#endif //SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_COLLISION_VELOCITYCHANGEDETECTOR_H_
