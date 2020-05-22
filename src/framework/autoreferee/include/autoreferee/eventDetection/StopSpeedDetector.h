//
// Created by rolf on 21-05-20.
//

#ifndef SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_STOPSPEEDDETECTOR_H_
#define SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_STOPSPEEDDETECTOR_H_

#include "eventDetection/SingleEventDetector.h"
class StopSpeedDetector : public SingleEventDetector {
 public:
  StopSpeedDetector();
  std::vector<proto::GameEvent> update(const Context & context) override;
  [[nodiscard]] bool isApplicable(const GameCommand& command) const override;
 private:
  const double SPEED_LIMIT = 1.5;
  std::set<RobotID> blueOffenders;
  Time lastBlueSendTime;
  std::set<RobotID> yellowOffenders;
  Time lastYellowSendTime;
  Time stopStartTime;
  const Time RESEND_TIME_INTERVAL = Time(2.0);
  const Time GRACE_PERIOD = Time(2.0);
};

#endif //SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_STOPSPEEDDETECTOR_H_
