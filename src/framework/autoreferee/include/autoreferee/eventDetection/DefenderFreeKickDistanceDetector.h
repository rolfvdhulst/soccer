//
// Created by rolf on 22-05-20.
//

#ifndef SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_DEFENDERFREEKICKDISTANCE_H_
#define SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_DEFENDERFREEKICKDISTANCE_H_

#include "SingleEventDetector.h"
class DefenderFreeKickDistanceDetector : public SingleEventDetector {
 public:
  DefenderFreeKickDistanceDetector();
  std::vector<proto::GameEvent> update(const Context & context) override;
  [[nodiscard]] bool isApplicable(const GameCommand& command) const override;
 private:
  const double FREEKICK_DISTANCE = 0.5;
};

#endif //SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_DEFENDERFREEKICKDISTANCE_H_
