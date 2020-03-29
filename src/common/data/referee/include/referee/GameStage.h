//
// Created by rolf on 19-03-20.
//

#ifndef SOCCER_GAMESTAGE_H
#define SOCCER_GAMESTAGE_H

#include <protobuf/ssl_referee.pb.h>
/**
 * @author Rolf
 * @brief Class that represents the game stage. This is the phase that the game is currently in.
 * It's modelled so that usage can essentially be as that for enum class but with some additional useful functions on top
 * You should be able to construct as GameStage stage = GameStage::NORMAL_FIRST_HALF
 */
class GameStage {
    public:
        enum Stage{
          // The first half is about to start.
          // A kickoff is called within this stage.
          // This stage ends with the NORMAL_START.
                  NORMAL_FIRST_HALF_PRE = 0,
          // The first half of the normal game, before half time.
                  NORMAL_FIRST_HALF = 1,
          // Half time between first and second halves.
                  NORMAL_HALF_TIME = 2,
          // The second half is about to start.
          // A kickoff is called within this stage.
          // This stage ends with the NORMAL_START.
                  NORMAL_SECOND_HALF_PRE = 3,
          // The second half of the normal game, after half time.
                  NORMAL_SECOND_HALF = 4,
          // The break before extra time.
                  EXTRA_TIME_BREAK = 5,
          // The first half of extra time is about to start.
          // A kickoff is called within this stage.
          // This stage ends with the NORMAL_START.
                  EXTRA_FIRST_HALF_PRE = 6,
          // The first half of extra time.
                  EXTRA_FIRST_HALF = 7,
          // Half time between first and second extra halves.
                  EXTRA_HALF_TIME = 8,
          // The second half of extra time is about to start.
          // A kickoff is called within this stage.
          // This stage ends with the NORMAL_START.
                  EXTRA_SECOND_HALF_PRE = 9,
          // The second half of extra time.
                  EXTRA_SECOND_HALF = 10,
          // The break before penalty shootout.
                  PENALTY_SHOOTOUT_BREAK = 11,
          // The penalty shootout.
                  PENALTY_SHOOTOUT = 12,
          // The game is over.
                  POST_GAME = 13
        };
        GameStage();
        constexpr GameStage(Stage stage);
        explicit GameStage(const proto::Referee::Stage& gameStage);
        operator Stage() const;// Allow switches and comparisons.
        // Putting constexpr here causes clang to  stop warning on incomplete case handling
        explicit operator bool() = delete; //Prevent if(GameStage) incorrect usage
        constexpr bool operator ==(GameStage other) const;
        constexpr bool operator !=(GameStage other) const;
        [[nodiscard]] constexpr bool isPreHalf() const;
        [[nodiscard]] constexpr bool isNormalPlayTime() const;
        [[nodiscard]] constexpr bool isExtendedPlayTime() const;
        [[nodiscard]] constexpr bool isBreak() const;
        [[nodiscard]] constexpr bool hasTime() const;

    private:
        Stage value =NORMAL_FIRST_HALF_PRE;
};

#endif //SOCCER_GAMESTAGE_H
