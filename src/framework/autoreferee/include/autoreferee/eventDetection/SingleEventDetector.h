//
// Created by rolf on 10-05-20.
//

#ifndef SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_SINGLEEVENTDETECTOR_H_
#define SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_SINGLEEVENTDETECTOR_H_

#include <protobuf/ssl_game_event.pb.h>
#include "Context.h"

class SingleEventDetector {
 public:
  virtual std::vector<proto::GameEvent> update(const Context& context) = 0;
  [[nodiscard]] virtual bool isApplicable(const GameCommand& command) const = 0;
  [[nodiscard]] bool canRetrigger(const Time& time) const;
  void trigger(const Time& time);
 protected:
  Time lastTriggerTime = Time(0.0);
  Time reTriggerTime = Time(2.0);
};

#endif //SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_SINGLEEVENTDETECTOR_H_
