//
// Created by rolf on 07-01-21.
//

#include "ball/kick/detection/KickEventFilter.h"
void KickEventFilter::addObservation(const BallObservation &observation) {
  velocity_checkers[observation.cameraID].addObservation(observation);

}
void KickEventFilter::clearCamera(int id) {
  velocity_checkers.erase(id);
}
std::optional<KickEvent> KickEventFilter::update(int cameraID,
                                                 const BallGroundFilter::ObservationPredictionPair &op_pair) {

  //take other collisions to make sure we do not misidentify other collisions
  for (const auto &collision : op_pair.prediction.collisions) {
    if (collision.type != CollisionChecker::CollisionType::ROBOT_FRONT) {
      last_other_collisions[cameraID] = collision;
      break;
    }
  }

  //take the first robot collision
  bool collisionPredicted = false;
  for (const auto &collision : op_pair.prediction.collisions) {
    if (collision.type == CollisionChecker::CollisionType::ROBOT_FRONT) {
      last_robot_collisions[cameraID] = collision;
      collisionPredicted = true;
      break;
    }
  }

  bool has_observation = op_pair.observation.has_value();
  if (has_observation) {
    addObservation(op_pair.observation.value());
    bool collisionObserved = velocity_checkers[cameraID].detectChange();

    //first determine if we have a recent collision
    std::optional<CollisionChecker::Collision> recent_collision;
    bool compute_kick_event = false;

    Time last_dt = velocity_checkers[cameraID].lastTimeDifference();
    Time check_from = op_pair.observation->timeCaptured - last_dt - last_dt;
    //TODO: maybe adjust for 3 frames leeway, but that might be too much
    //TODO: make multiply operators with int for time class?
    auto last_other_collision = last_other_collisions.find(cameraID);
    bool recent_other_collision = last_other_collision != last_other_collisions.end()
        && last_other_collision->second.collisionTime >= check_from;
    if (!collisionPredicted) {
      auto last_robot_collision = last_robot_collisions.find(cameraID);
      if (last_robot_collision != last_robot_collisions.end()) {
        if (last_robot_collision->second.collisionTime >= check_from) {
          recent_collision = last_robot_collision->second;
        }else if(!recent_other_collision){
          compute_kick_event = true;
        }
      }else if(!recent_other_collision){
        compute_kick_event = true;
      }
    }else {
      recent_collision = last_robot_collisions[cameraID];
    }

    if(recent_collision.has_value()){
      CollisionState type = velocity_checkers[cameraID].estimateCollision(recent_collision.value());
      if(collisionObserved && type == CollisionState::COLLIDED){
        KickEvent kick_event;
        auto coll_type = recent_collision->type;
        if(coll_type == CollisionChecker::CollisionType::ROBOT_FRONT){
          kick_event.type;

        }else if(coll_type == CollisionChecker::CollisionType::ROBOT_HULL){

        }else{
          kick_event.type = UNKNOWN;
        }
        kick_event.initial_pos = Eigen::Vector3d(recent_collision->position.x(),recent_collision->position.y(),0.02133);//TODO: fix ball constant
//        kick_event.initial_vel = (op_pair.observation->position-recent_collision->position) /
//        kick_event.time = recent_collision->collisionTime;
        return kick_event;
      }else{
        //predicted collision:
      }

    }else if(compute_kick_event && collisionObserved){ // There was probably a collision but from unknown causes
      KickEvent kick_event;
      kick_event.type = KickEventType::UNKNOWN;
    }else{

    }

      //if a collision was observed, there's two cases for which we assume the ball was actually kicked
      //First, if there was a collision event with a robot front around that time and location, we assume a kick happened.
      //Second, if NO other collision event type was seen at all, we also assume a kick to make sure vision problems do not bother us.

      if (collisionPredicted && collisionObserved) { //If a collision was both predicted AND observed
        KickEvent kick_event;

      } else if (!collisionPredicted && collisionObserved) {
        //Check predicted collisions from previous ticks


      } else if (collisionPredicted && !collisionObserved) {

      }
      //If we have a recent kick event from another camera, we add the information
    }
    return std::nullopt;
  }
