//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_WORLDSTATE_H
#define SOCCER_WORLDSTATE_H

#include <core/Time.h>
#include <optional>
#include "BallState.h"
#include "RobotState.h"
#include "WorldTeam.h"

/**
 * @author Rolf van der Hulst
 * @brief This class represents the entire world state.
 * One should never cast away the const references which are returned. This class should remain constant
 * If you want to edit e.g. a robot vector, copy it.
 */
class WorldState {
    public:
        const Time& getTime();
        //TODO: Copies for now... we might want to look later if we can make these references using views or so, and what the performance difference actually is
        //TODO: we can avoid copies also with e.g. const BallState * (using pointers)
        [[nodiscard]] std::optional<BallState> getBall() const;
        [[nodiscard]] std::optional<RobotState> getRobot(const RobotID &id, WorldTeam team) const;
        [[nodiscard]] std::optional<RobotState> getOurRobot(const RobotID &id) const;
        [[nodiscard]] std::optional<RobotState> getTheirRobot(const RobotID &id) const;

        [[nodiscard]] const std::vector<RobotState>& getTeam(WorldTeam team) const;
        [[nodiscard]] const std::vector<RobotState>& getUs() const;
        [[nodiscard]] const std::vector<RobotState>& getThem() const;
        [[nodiscard]] const std::vector<RobotState>& getAllRobots() const;

        [[nodiscard]] unsigned int ourRobotCount() const;
        [[nodiscard]] unsigned int theirRobotCount() const;

        [[nodiscard]] bool weHaveRobots() const;
        [[nodiscard]] bool theyHaveRobots() const;
        [[nodiscard]] bool hasBall() const;
        [[nodiscard]] bool hasRobots() const;
    private:
        Time time;
        std::optional<BallState> ball;
        std::vector<RobotState> us;
        std::vector<RobotState> them;
        std::vector<RobotState> robots;//All robots, ordered as us+them
};

#endif  // SOCCER_WORLDSTATE_H
