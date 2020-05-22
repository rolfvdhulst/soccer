//
// Created by rolf on 22-05-20.
//

#include "eventDetection/DefenderFreeKickDistanceDetector.h"
bool DefenderFreeKickDistanceDetector::isApplicable(const GameCommand &command) const {
  return command.isFreeKick() || command.isFreeKick();
}
DefenderFreeKickDistanceDetector::DefenderFreeKickDistanceDetector() : SingleEventDetector(){

}
std::vector<proto::GameEvent> DefenderFreeKickDistanceDetector::update(const Context &context) {
  bool opponentIsBlue = GameCommand::DIRECT_FREE_THEM == context.referee.command
      || GameCommand::DIRECT_FREE_THEM == context.referee.command
      || GameCommand::KICKOFF_THEM == context.referee.command;
  if(context.worldHistory.empty()){
    return {};
  }
  const WorldState & world = context.currentWorld();
  if(!world.hasBall()){
    return {};
  }
  const std::vector<RobotState> robots = opponentIsBlue ? world.getUs() : world.getThem();
  for(const auto& robot : robots){
    double distance = (world.getBall()->pos()-robot.pos()).length()-robot.radius()-world.getBall()->radius();
    //in addition to the distance check we also check if the ball is moving because this is likely in some scenario's where it might have just been kicked
    if(distance < FREEKICK_DISTANCE && world.getBall()->vel().length()<0.8 && canRetrigger(world.getTime())){
      proto::GameEvent event;
      event.set_type(proto::DEFENDER_TOO_CLOSE_TO_KICK_POINT);
      proto::GameEvent_DefenderTooCloseToKickPoint * violation = event.mutable_defender_too_close_to_kick_point();
      violation->set_by_team(opponentIsBlue ? proto::Team::BLUE : proto::Team::YELLOW);
      violation->set_by_bot(robot.id().getID());
      violation->mutable_location()->set_x(robot.pos().x());
      violation->mutable_location()->set_y(robot.pos().y());
      violation->set_distance(distance);
      trigger(world.getTime());

      return {event};
    }
  }
  return {};
}
