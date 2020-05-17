//
// Created by rolf on 09-05-20.
//

#include "eventDetection/EventDetector.h"
#include <referee/GameState.h>
#include "eventDetection/BallPlacementDetector.h"

std::vector<proto::GameEvent> EventDetector::update(const proto::World& world,const proto::GameState &gameState,
                                                    const std::optional<proto::SSL_GeometryData>& geometry) {
  //update context info

  if(geometry){
    context.geometry = FieldState(geometry->field(),DefaultField::DivisionA);
  }
  WorldState newWorld(world,true);
  context.worldHistory.emplace_front(newWorld);
  if((newWorld.getTime()-context.worldHistory.back().getTime()>Time(2.0))){
    context.worldHistory.pop_back();
  }
  GameState game_state(gameState);
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
    std::unique_ptr<SingleEventDetector> ballPlacementDetector= std::make_unique<BallPlacementDetector>();
    detectors.push_back(std::move(ballPlacementDetector));
}
