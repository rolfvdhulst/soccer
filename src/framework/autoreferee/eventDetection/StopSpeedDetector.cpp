//
// Created by rolf on 21-05-20.
//

#include "eventDetection/StopSpeedDetector.h"
StopSpeedDetector::StopSpeedDetector() : SingleEventDetector(){
}
//TODO: perhaps register robots and count time (and have e.g. 200/300 ms for them to be too fast before we register)
std::vector<proto::GameEvent> StopSpeedDetector::update(const Context &context) {
  if(context.worldHistory.empty()){
    return {};
  }

  const WorldState& world = context.currentWorld();
  const Time& currentTime = world.getTime();
  //if a new stop state, we reset all data
  if (context.commandChanged) {
    blueOffenders.clear();
    yellowOffenders.clear();
    //We pretend they are sent at the start of the period
    lastBlueSendTime = currentTime;
    lastYellowSendTime = currentTime;
    stopStartTime = currentTime;
  }
  //In the first 2 seconds after the stop command we don't send commands
  if(currentTime-stopStartTime< GRACE_PERIOD){
    return {};
  }
  std::vector<proto::GameEvent> events;
  //Check blue robot speeds
  for(const auto& robot : world.getUs()){
    if(robot.vel().length()>SPEED_LIMIT && (currentTime-lastBlueSendTime)>RESEND_TIME_INTERVAL && !blueOffenders.count(robot.id())){
      lastBlueSendTime = currentTime;
      blueOffenders.insert(robot.id());
      proto::GameEvent event;
      proto::GameEvent_BotTooFastInStop * violation = event.mutable_bot_too_fast_in_stop();
      violation->set_by_bot(robot.id().getID());
      violation->set_by_team(proto::Team::BLUE);
      violation->set_speed(robot.vel().length());
      violation->mutable_location()->set_x(robot.pos().x());
      violation->mutable_location()->set_y(robot.pos().y());
      events.push_back(event);
      break;
    }
  }
  //Check yellow robot speeds;
  for(const auto& robot : world.getThem()){
    if(robot.vel().length()>SPEED_LIMIT && (currentTime-lastYellowSendTime)>RESEND_TIME_INTERVAL && !yellowOffenders.count(robot.id())){
      lastYellowSendTime = currentTime;
      yellowOffenders.insert(robot.id());
      proto::GameEvent event;
      proto::GameEvent_BotTooFastInStop * violation = event.mutable_bot_too_fast_in_stop();
      violation->set_by_bot(robot.id().getID());
      violation->set_by_team(proto::Team::YELLOW);
      violation->set_speed(robot.vel().length());
      violation->mutable_location()->set_x(robot.pos().x());
      violation->mutable_location()->set_y(robot.pos().y());
      events.push_back(event);
      break;
    }
  }
  return events;
}
bool StopSpeedDetector::isApplicable(const GameCommand &command) const {
  return GameCommand::STOP == command;
}
