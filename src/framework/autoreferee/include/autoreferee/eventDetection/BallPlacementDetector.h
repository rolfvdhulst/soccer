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
        /**
         * The maximum distance robots are allowed to be from the ball.
         * This differs based on the referee command
         * @param context
         * @return
         */
        double minimumRobotDistance(const Context &context);
        const double TOLERANCE_RADIUS = 0.15; //as defined by the rules
        const double STATIONARY_BALL_SPEED = 0.1; //a tolerance treshhold to ensure stationarity
        std::optional<Vector2> startingPos;
        std::optional<double> startingDistance;
        Time startingTime;

};

#endif //SOCCER_BALLPLACEMENTDETECTOR_H
