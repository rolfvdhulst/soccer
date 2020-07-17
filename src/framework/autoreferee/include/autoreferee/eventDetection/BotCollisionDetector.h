//
// Created by rolf on 17-07-20.
//

#ifndef SOCCER_BOTCOLLISIONDETECTOR_H
#define SOCCER_BOTCOLLISIONDETECTOR_H

#include "SingleEventDetector.h"
class BotCollisionDetector : public SingleEventDetector{
public:
    BotCollisionDetector();
    std::vector<proto::GameEvent> update(const Context & context) override;
    [[nodiscard]] bool isApplicable(const GameCommand& command) const override;
private:

    const double COLLISION_SPEED=1.5;
    enum Offender{
        BLUE,
        YELLOW,
        BOTH
    };
    struct Collision{
        Collision(const Time& time, RobotState  blue, RobotState  yellow, bool blueFaster, const Vector2& collisionLocation, double projectedVelocity, double absoluteVelocityDifference);
        Time collisionTime;
        RobotState blueRobot;
        RobotState yellowRobot;
        Offender offender;
        Vector2  collisionLocation;
        double projectedVelocity;
        double absoluteVelocityDifference;

    };
    std::vector<Collision> oldCollisions;
    std::vector<Collision> detectCollisions(const WorldState& world);
    [[nodiscard]] std::optional<Collision> getCollision(const Time& collisionTime,const RobotState& blueBot, const RobotState& yellowBot) const;
    std::vector<Collision> getNewCollisions(const Time& currentTime, const std::vector<Collision>& detectedCollisions);
    proto::GameEvent convertToProtoEvent(const Collision& collision);
};


#endif //SOCCER_BOTCOLLISIONDETECTOR_H
