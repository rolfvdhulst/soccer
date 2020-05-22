//
// Created by rolf on 21-05-20.
//

#include "eventDetection/BallPlacementInterferenceDetector.h"
#include <math/geometry/Tube.h>
bool BallPlacementInterferenceDetector::isApplicable(const GameCommand &command) const {
  return command.isBallPlacement();
}
BallPlacementInterferenceDetector::BallPlacementInterferenceDetector() : SingleEventDetector() {
  reTriggerTime = Time(2.0);
}
std::vector<proto::GameEvent> BallPlacementInterferenceDetector::update(const Context &context) {
  const WorldState &world = context.currentWorld();
  const Time currentTime = world.getTime();
  //If there is no recent world information we don't send anything
  if (context.worldHistory.empty()) {
    return {};
  }
  if (!world.hasBall() || context.referee.designatedPosition == std::nullopt) {
    return {};
  }
  BallState ball = *world.getBall();
  Vector2 placementPos = *context.referee.designatedPosition;
  //Create line from which we measure the legal distance
  LineSegment interferenceLine(ball.pos(),placementPos);
  //check robots of opposite color of the color placing
  bool opposingTeamIsBlue = GameCommand::BALL_PLACEMENT_US != context.referee.command;
  const std::vector<RobotState>& robots =  opposingTeamIsBlue ? world.getUs() : world.getThem();
  removeOffendersNotInWorld(robots);
  for(const auto& robot : robots){
    auto offender = offenders.find(robot.id());
    if(interferenceLine.distanceTo(robot.pos())<= BALLLINE_DISTANCE + robot.radius()){
      if(offender == offenders.end()){
        Offender newOffender;
        newOffender.id = robot.id();
        newOffender.offendedTime = currentTime;
        newOffender.lastPos = robot.pos();
        offenders.insert(std::make_pair(newOffender.id,newOffender));
      }else{
        //update last pos we saw offender
        offender->second.lastPos = robot.pos();
      }
    } else{
      //offender moved out of the area so we can remove it from our list
      if (offender != offenders.end()){
        offenders.erase(offender);
      }
    }
  }
  std::vector<proto::GameEvent> events = getEvents(opposingTeamIsBlue,currentTime);
  return events;
}
void BallPlacementInterferenceDetector::removeOffendersNotInWorld(const std::vector<RobotState>& robots) {
  auto offender = offenders.begin();
  while(offender != offenders.end()){
    bool foundOffender = false;
    for(const auto& robot : robots){
      if(robot.id() == offender->first){
        foundOffender = true;
        break;
      }
    }
    if(!foundOffender){
      offender = offenders.erase(offender);
    }else{
      offender ++;
    }
  }
}
std::vector<proto::GameEvent> BallPlacementInterferenceDetector::getEvents(bool blueIsOffender, Time currentTime) {
  std::vector<proto::GameEvent> events;
  if(!canRetrigger(currentTime)){
    return events;
  }
  //As specified by the rules, we only send one robot every two seconds even if multiple robots are breaking the rule.
  //The two seconds are handled by the trigger/retrigger functions
  for (auto& offender : offenders){
    if(offender.second.shouldSend(currentTime)){
      proto::GameEvent event;
      proto::GameEvent_BotInterferedPlacement * interference = event.mutable_bot_interfered_placement();
      interference->set_by_bot(offender.second.id.getID());
      interference->set_by_team(blueIsOffender ? proto::Team::BLUE : proto::Team::YELLOW);
      proto::Location location;
      location.set_x(offender.second.lastPos.x());
      location.set_y(offender.second.lastPos.y());
      interference->mutable_location()->CopyFrom(location);
      events.push_back(event);
      trigger(currentTime);
      return events;
    }
  }
  return events;
}
