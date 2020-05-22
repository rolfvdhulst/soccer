//
// Created by rolf on 17-5-20.
//

#ifndef SOCCER_BALLPLACEMENTDETECTOR_H
#define SOCCER_BALLPLACEMENTDETECTOR_H

#include "SingleEventDetector.h"
class BallPlacementDetector : public SingleEventDetector{
    public:
        BallPlacementDetector();
        std::vector<proto::GameEvent> update(const Context & context) override;
        [[nodiscard]] bool isApplicable(const GameCommand& command) const override;
    private:
        const double TOLERANCE_RADIUS = 0.15; //as defined by the rules
        const double STATIONARY_BALL_SPEED = 0.1; //a tolerance treshhold to ensure stationarity
        std::optional<Vector2> startingPos;
        std::optional<double> startingDistance;
        Time startingTime;

  bool robotsFarAway(std::optional<GameCommand> nextCommand, const WorldState &world, const BallState &ball) const;
  void resetStartparams(const Context &context, const WorldState &world);
  proto::GameEvent makeFailure(const Context &context, const WorldState &world);
  proto::GameEvent makeSuccess(const Context &context, const WorldState &world, double remainingDistance);
};

#endif //SOCCER_BALLPLACEMENTDETECTOR_H
