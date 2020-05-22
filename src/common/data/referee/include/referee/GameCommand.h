//
// Created by rolf on 27-03-20.
//

#ifndef SOCCER_GAMECOMMAND_H
#define SOCCER_GAMECOMMAND_H

#include <protobuf/GameState.pb.h>

class GameCommand {
    public:
        enum Command{
          // All robots should completely stop moving.
                  HALT = 0,
          // Robots must keep 50 cm from the ball.
                  STOP = 1,
          // Game is running normally
                  RUNNING = 2,
          // They may move into kickoff position.
                  PREPARE_KICKOFF_THEM = 4,
          // We may move into kickoff position.
                  PREPARE_KICKOFF_US = 5,
          // They may move into penalty position.
                  PREPARE_PENALTY_THEM = 6,
          // We may move into penalty position.
                  PREPARE_PENALTY_US = 7,
          // They may take a direct free kick.
                  DIRECT_FREE_THEM = 8,
          // We may take a direct free kick.
                  DIRECT_FREE_US = 9,
          // They may take an indirect free kick.
                  INDIRECT_FREE_THEM = 10,
          // We may take an indirect free kick.
                  INDIRECT_FREE_US = 11,
          // They are currently in a timeout.
                  TIMEOUT_THEM = 12,
          // We are currently in a timeout.
                  TIMEOUT_US = 13,
          // Equivalent to STOP, but They must pick up the ball and
          // drop it in the Designated Position.
                  BALL_PLACEMENT_THEM = 16,
          // Equivalent to STOP, but We must pick up the ball and drop
          // it in the Designated Position.
                  BALL_PLACEMENT_US = 17,

                  KICKOFF_US = 18,
                  KICKOFF_THEM = 19,
                  PENALTY_US = 20,
                  PENALTY_THEM =21
        };
        GameCommand();
        constexpr GameCommand(Command command);
        explicit GameCommand(const proto::RefereeState::Command &command);
        operator Command() const;// Allow switches and comparisons.
        // Putting constexpr here causes clang to  stop warning on incomplete case handling
        explicit operator bool() = delete; //Prevent if(GameStage) incorrect usage
        [[nodiscard]] std::string toString() const;
        bool operator ==(const GameCommand& other) const;
        bool operator !=(const GameCommand& other) const;
        [[nodiscard]] bool isBallPlacement() const;
        [[nodiscard]] bool isFreeKick() const;
        [[nodiscard]] bool isKickOff() const;
    private:
        Command value;
};

#endif //SOCCER_GAMECOMMAND_H
