//
// Created by rolf on 09-05-20.
//

#ifndef SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_EVENTDETECTOR_H_
#define SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_EVENTDETECTOR_H_

#include <vector>
#include <memory>
#include <protobuf/World.pb.h>
#include <protobuf/messages_robocup_ssl_geometry.pb.h>
#include <referee/GameState.h>
#include "SingleEventDetector.h"
#include "Context.h"
/**
 * Attempts to detect if any events are taking place
 */
class EventDetector {
 public:
  EventDetector();
  std::vector<proto::GameEvent> update(const proto::World& world, const proto::GameState &gameState, const std::optional<proto::SSL_GeometryData>& geometry);
 private:
  Context context;

  std::vector<std::unique_ptr<SingleEventDetector>> detectors;

};

#endif //SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_EVENTDETECTOR_H_
