//
// Created by rolf on 06-07-20.
//

#ifndef SOCCER_ATTACKERTODEFENSEAREADISTANCEDETECTOR_H
#define SOCCER_ATTACKERTODEFENSEAREADISTANCEDETECTOR_H

#include "SingleEventDetector.h"

//We store offenders for
class AttackerToDefenseAreaDistanceDetector : public SingleEventDetector{
public:
    AttackerToDefenseAreaDistanceDetector();
    std::vector<proto::GameEvent> update(const Context & context) override;
    [[nodiscard]] bool isApplicable(const GameCommand& command) const override;
private:
    void removeOldOffenders(bool blue, const Time& currentTime );
    std::vector<proto::GameEvent> checkRobots(bool blue, const Time& currentTime, const Context& context);
    [[nodiscard]] bool hasRecentlyOffended(bool blue, RobotID id) const;

    const double DEFENSE_AREA_DISTANCE = 0.2; // [m]
    const double ERROR_MARGIN = 0.02;// [m]
    const Time GRACE_PERIOD = Time(2.0); // [s]
    const Time OFFENSE_RESEND_TIME = Time(2.0);// [s]
    Time gamestateStartTime;

    std::map<RobotID,Time> yellowOffenders;
    std::map<RobotID,Time> blueOffenders;


};


#endif //SOCCER_ATTACKERTODEFENSEAREADISTANCEDETECTOR_H
