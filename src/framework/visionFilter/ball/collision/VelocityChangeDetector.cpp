//
// Created by rolf on 07-01-21.
//

#include "ball/collision/VelocityChangeDetector.h"
#include "math/geometry/Vector2.h"
#include "math/geometry/Angle.h"

bool VelocityChangeDetector::detectChange() {
  bool thisIsKick = false;
  if(detectKick()){
    thisIsKick = !previousWasKick;
  }
  previousWasKick = thisIsKick;
  return thisIsKick;
}
bool VelocityChangeDetector::detectKick() const {
  const BallObservation& first = buffer.at(0);
  const BallObservation& second = buffer.at(1);
  const BallObservation& third = buffer.at(2);

  double firstTimeDiff = (second.timeCaptured-first.timeCaptured).asSeconds();
  double secondTimeDiff = (third.timeCaptured-second.timeCaptured).asSeconds();
  Eigen::Vector2d firstVel = (second.position - first.position)/firstTimeDiff;
  Eigen::Vector2d secondVel = (third.position-second.position)/secondTimeDiff;

  //Check if velocity changed significantly
  double speedDiff = secondVel.norm()-firstVel.norm();
  // absolute difference in angle
  double angleDiff=abs(Vector2(firstVel).toAngle().angleDiff(Vector2(secondVel).toAngle()));
  const double rollingDeviation = toRadians(10.0); // degrees
  const double speedDeviation = 1.0;
  bool angleChangedSignificantly= angleDiff > rollingDeviation;
  bool speedChangedSignificantly = abs(speedDiff) > speedDeviation;
  return angleChangedSignificantly || speedChangedSignificantly;
}
void VelocityChangeDetector::addObservation(const BallObservation &observation) {
  buffer.push_back(observation);
}
CollisionState VelocityChangeDetector::estimateCollision(const CollisionChecker::Collision &collision) const {
  const BallObservation& first = buffer.at(0);
  const BallObservation& second = buffer.at(1);
  const BallObservation& third = buffer.at(2);
  //First check if it makes any sense to estimate the collision
  if (collision.collisionTime >third.timeCaptured || collision.collisionTime < first.timeCaptured){
    return CollisionState::UNOBSERVED;
  }
  double extrapolation_time =(third.timeCaptured-collision.collisionTime).asSeconds();
  //coordinate system relative to the predicted collision location
  Vector2 no_collision_prediction_rel =  collision.inVelocity * extrapolation_time;
  Vector2 collision_prediction_rel =  collision.outVelocity * extrapolation_time;

  Vector2 collision_observation = Vector2(third.position)-collision.position;
  double no_collision_dist = collision_observation.dist(no_collision_prediction_rel);
  double collision_dist = collision_observation.dist(collision_prediction_rel);
  if(no_collision_dist<0.02 && collision_dist < 0.02){
    return CollisionState::UNCERTAIN;
  }
  bool close_to_nocol = no_collision_dist < 0.05 && no_collision_dist < collision_dist;
  bool close_to_col = collision_dist < 0.05 && collision_dist < no_collision_dist;

  Vector2 normal = collision.normal_dir;
  //if somehow the normal and incoming velocity are weird, we rotate the normal with 180 degrees. Shouldn't happen, but might if collision produces
  // a normal facing the wrong direction.
  if(abs(Angle(normal).angleDiff(Angle(collision.inVelocity))) < M_PI_2){
    normal = normal.rotate(M_PI);
  }
  //angle is in a reasonable cone depending on the normal and estimated outgoing velocity
  double normal_diff = Angle(normal).angleDiff(Angle(collision_prediction_rel));
  double collision_side_bound =  normal_diff < 0.0 ? normal_diff - toDegrees(15.0) : normal_diff + toDegrees(15.0);
  collision_side_bound = std::clamp(collision_side_bound,-toDegrees(90.0),toDegrees(90.0));
  double other_bound = collision_side_bound < 0.0 ? toDegrees(45.0) : -toDegrees(45.0);
  double negative_bound = fmin(collision_side_bound,other_bound);
  double positive_bound = fmax(collision_side_bound,other_bound);
  double actual_diff = Angle(normal).angleDiff(Angle(collision_observation));
  bool in_kick_angle = negative_bound < actual_diff && actual_diff < positive_bound;

  double no_collision_angle_diff = abs(Angle(no_collision_prediction_rel).angleDiff(Angle(collision_observation)));
  bool in_nocollision_angle = no_collision_angle_diff < toDegrees(20.0) && actual_diff >= M_PI_2;

  if (close_to_col && in_kick_angle){
    return CollisionState::COLLIDED;
  }else if(close_to_nocol && in_nocollision_angle){
    return CollisionState::NO_COLLISION;
  }
  if(in_kick_angle){
    return CollisionState::COLLIDED;
  }
  if(close_to_nocol){
    return CollisionState::NO_COLLISION;
  }
  return CollisionState::UNCERTAIN;
}
Time VelocityChangeDetector::lastTimeDifference() const {
  return buffer.at(0).timeCaptured-buffer.at(1).timeCaptured;
}
