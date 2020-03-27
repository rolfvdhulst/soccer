//
// Created by rolf on 27-03-20.
//

#ifndef SOCCER_GAMECOMMAND_H
#define SOCCER_GAMECOMMAND_H

class GameCommand {
    public:
        enum Command{
          // All robots should completely stop moving.
                  HALT = 0,
          // Robots must keep 50 cm from the ball.
                  STOP = 1,
          // A prepared kickoff or penalty may now be taken.
                  NORMAL_START = 2,
          // The ball is dropped and free for either team.
                  FORCE_START = 3,
          // The yellow team may move into kickoff position.
                  PREPARE_KICKOFF_YELLOW = 4,
          // The blue team may move into kickoff position.
                  PREPARE_KICKOFF_BLUE = 5,
          // The yellow team may move into penalty position.
                  PREPARE_PENALTY_YELLOW = 6,
          // The blue team may move into penalty position.
                  PREPARE_PENALTY_BLUE = 7,
          // The yellow team may take a direct free kick.
                  DIRECT_FREE_YELLOW = 8,
          // The blue team may take a direct free kick.
                  DIRECT_FREE_BLUE = 9,
          // The yellow team may take an indirect free kick.
                  INDIRECT_FREE_YELLOW = 10,
          // The blue team may take an indirect free kick.
                  INDIRECT_FREE_BLUE = 11,
          // The yellow team is currently in a timeout.
                  TIMEOUT_YELLOW = 12,
          // The blue team is currently in a timeout.
                  TIMEOUT_BLUE = 13,
          // The yellow team just scored a goal.

          // Equivalent to STOP, but the yellow team must pick up the ball and
          // drop it in the Designated Position.
                  BALL_PLACEMENT_YELLOW = 16,
          // Equivalent to STOP, but the blue team must pick up the ball and drop
          // it in the Designated Position.
                  BALL_PLACEMENT_BLUE = 17,

                  KICKOFF_BLUE,
                  KICKOFF_YELLOW,
                  PENALTY_BLUE,
                  PENALTY_YELLOW
        };
        constexpr GameCommand(Command command);
        operator Command() const;// Allow switches and comparisons.
        // Putting constexpr here causes clang to  stop warning on incomplete case handling
        explicit operator bool() = delete; //Prevent if(GameStage) incorrect usage
        constexpr bool operator ==(GameCommand other) const;
        constexpr bool operator !=(GameCommand other) const;

    private:
        Command value;
};

#endif //SOCCER_GAMECOMMAND_H
