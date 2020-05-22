//
// Created by rolf on 21-05-20.
//

#ifndef SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_BALLPLACEMENTINTERFERENCEDETECTOR_H_
#define SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_BALLPLACEMENTINTERFERENCEDETECTOR_H_

#include "SingleEventDetector.h"
class BallPlacementInterferenceDetector : public SingleEventDetector{
 public:
  BallPlacementInterferenceDetector();
  std::vector<proto::GameEvent> update(const Context & context) override;
  [[nodiscard]] bool isApplicable(const GameCommand& command) const override;
 private:
  struct Offender{
    RobotID id;
    Vector2 lastPos;
    Time offendedTime = Time(0.0);
    bool shouldSend(const Time& currentTime) const {
      return (currentTime-offendedTime)>Time(2.0);//repeat every 2 seconds;
    }
  };
  const double BALLLINE_DISTANCE = 0.5;
  std::map<RobotID,Offender> offenders;
  void removeOffendersNotInWorld(const std::vector<RobotState>& robots);
  std::vector<proto::GameEvent> getEvents(bool blueIsOffender,Time currentTime);
};

#endif //SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_BALLPLACEMENTINTERFERENCEDETECTOR_H_
