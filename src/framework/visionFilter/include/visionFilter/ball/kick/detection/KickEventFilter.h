//
// Created by rolf on 07-01-21.
//

#ifndef SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_KICK_DETECTION_KICKEVENTFILTER_H_
#define SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_KICK_DETECTION_KICKEVENTFILTER_H_

//Class which takes information from multiple camera's to determine whether a KickEvent Occurred
//KickEvents occur when the ball is both predicted to collide with a robot and then is also observed to have a significant change in velocity or direction
#include "ball/kick/detection/KickEvent.h"
#include "ball/BallGroundFilter.h"
#include "ball/collision/VelocityChangeDetector.h"

struct GroupedCollisions{
  std::map<int,CollisionChecker::Collision> camera_robot_collisions;
};
class KickEventFilter {
 public:
  void addObservation(const BallObservation& observation);
  void clearCamera(int id);
  std::optional<KickEvent> update(int cameraID, const BallGroundFilter::ObservationPredictionPair& op_pair );
 private:
  std::map<int,CollisionChecker::Collision> last_robot_collisions;
  std::map<int,CollisionChecker::Collision> last_other_collisions;
  std::map<int,VelocityChangeDetector> velocity_checkers;

  std::optional<KickEvent> last_event;
};

#endif //SOCCER_SRC_FRAMEWORK_VISIONFILTER_BALL_KICK_DETECTION_KICKEVENTFILTER_H_
