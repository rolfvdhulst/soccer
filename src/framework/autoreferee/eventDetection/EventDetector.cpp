//
// Created by rolf on 09-05-20.
//

#include "EventDetector.h"
std::vector<proto::GameEvent> EventDetector::update(const proto::World& world,
                                                    const std::optional<proto::SSL_GeometryData>& geometry) {
  //update context info
  if(geometry){
    context.geometry = FieldState(geometry->field(),DefaultField::DivisionA);
  }
  context.worldHistory.push_back(WorldState(world));
  //run all event detectors (if applicable)
  std::vector<proto::GameEvent> detectedEvents;
  for(const auto &detector : detectors){
    if(detector->isApplicable(context.refstate)){
      auto events = detector->update(context);
      if(!events.empty()){
        detectedEvents.insert(detectedEvents.end(),events.begin(),events.end());
      }
    }
  }
  return detectedEvents;
}
EventDetector::EventDetector() {
  detectors = {
      std::make_unique();//EventDetector
  };
}
