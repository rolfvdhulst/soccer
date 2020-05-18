//
// Created by rolf on 09-05-20.
//

#include "eventDetection/EventDetector.h"
#include <referee/GameState.h>
#include "eventDetection/BallPlacementDetector.h"

std::vector<proto::GameEvent> EventDetector::update(
        const proto::World& world,const proto::GameState &gameState,
        const std::optional<proto::SSL_GeometryData>& geometry,
        const proto::TeamRobotInfo &robotInfo) {
  //update context info

  if(geometry){
    context.geometry = FieldState(geometry->field(),DefaultField::DivisionA);
  }
  WorldState newWorld(world,true,robotInfo);
  context.worldHistory.emplace_front(newWorld);
  if((newWorld.getTime()-context.worldHistory.back().getTime()>Time(2.0))){
    context.worldHistory.pop_back();
  }
  GameState game_state(gameState);
  // check if commandChanged
  context.commandChanged = game_state.refState->command != context.referee.command;
  context.referee=*(game_state.refState);

  //run all event detectors (if applicable)
  std::vector<proto::GameEvent> detectedEvents;
  for(const auto &detector : detectors){
    if(detector->isApplicable(context.referee.command)){
      auto events = detector->update(context);
      if(!events.empty()){
        detectedEvents.insert(detectedEvents.end(),events.begin(),events.end());
      }
    }
  }
  return detectedEvents;
}
EventDetector::EventDetector() {
    detectors.clear();
    //ball placement detector detects ball placement success and failure
    std::unique_ptr<SingleEventDetector> ballPlacementDetector= std::make_unique<BallPlacementDetector>();
    detectors.push_back(std::move(ballPlacementDetector));
    //ball placement interference detects if the team not placing the ball is interfering ball placement in any way

}
