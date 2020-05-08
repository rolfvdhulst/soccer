//
// Created by rolf on 28-03-20.
//

#ifndef SOCCER_GAMEEVENT_H
#define SOCCER_GAMEEVENT_H
#include <variant>
#include <protobuf/ssl_game_event.pb.h>
#include <world/RobotID.h>
#include <math/geometry/Vector2.h>
#include <core/Time.h>
#include "Team.h"

struct BallLeftField{
  BallLeftField(const proto::GameEvent_BallLeftField& event, bool atGoalLine);
  [[nodiscard]] std::string toString() const;
  Team team; //team that last touched ball
  std::optional<RobotID> botID; // bot that last touched ball
  std::optional<Vector2> location; // location where ball went out of field
  bool throughGoalLine;
};
struct AimlessKick{
  AimlessKick(const proto::GameEvent_AimlessKick& event);
  [[nodiscard]] std::string toString() const;
  Team team; //team that last touched ball
  std::optional<RobotID> botID; // bot that last touched ball
  std::optional<Vector2> location; // location where ball went out of field
  std::optional<Vector2> kickLocation;
};
struct Goal{
  Goal(const proto::GameEvent_Goal& event);
  [[nodiscard]] std::string toString() const;

  Team team; // the team that scored the goal (e.g for which the goal counts)
  std::optional<Team> kickingTeam;  // the team that shot the goal (different from by_team for own goals)
  std::optional<RobotID> botID; //robot that shot the goal
  std::optional<Vector2> location;//where the ball entered the goal
  std::optional<Vector2> kickLocation;// where the ball was kicked from
};
// the ball entered the goal directly during an indirect free kick
struct IndirectGoal{
  IndirectGoal(const proto::GameEvent_IndirectGoal& event);
  [[nodiscard]] std::string toString() const;
  Team team; // the team that scored the goal (e.g for which the goal counts)
  std::optional<RobotID> botID; //robot that shot the goal
  std::optional<Vector2> location;//where the ball etnered the goal
  std::optional<Vector2> kickLocation;// where the ball was kicked from
};
struct ChippedGoal{
  ChippedGoal(const proto::GameEvent_ChippedGoal& event);
  [[nodiscard]] std::string toString() const;
  Team team; // the team that scored the goal
  std::optional<RobotID> botID; //robot that shot the goal
  std::optional<Vector2> location;//where the ball entered the goal
  std::optional<Vector2> kickLocation;// where the ball was kicked from
  std::optional<double> ballHeight;
};
struct BotTooFastInStop{
  BotTooFastInStop(const proto::GameEvent_BotTooFastInStop& event);
  [[nodiscard]] std::string toString() const;
  Team team; // the team that is guilty
  std::optional<RobotID> botID; //robot which drove to fast
  std::optional<Vector2> location;//position of the robot
  std::optional<double> speed;// speed of the robot that drove too fast
};
struct DefenderTooCloseToKickPoint{
  DefenderTooCloseToKickPoint(const proto::GameEvent_DefenderTooCloseToKickPoint& event);
  [[nodiscard]] std::string toString() const;
  Team team; // the team that is guilty
  std::optional<RobotID> botID; //robot which violated distance rule
  std::optional<Vector2> location;//position of the robot
  std::optional<double> distance;//distance to the kickPoint
};
struct BotCrashDrawn{
  BotCrashDrawn(const proto::GameEvent_BotCrashDrawn& event);
  [[nodiscard]] std::string toString() const;
  std::optional<RobotID> yellowID;
  std::optional<RobotID> blueID;
  std::optional<Vector2> crashPosition;
  std::optional<double> crashSpeed;
  std::optional<double> speedDifference;
  std::optional<double> crashAngle; //in radians
};
struct BotCrashUnique{
  BotCrashUnique(const proto::GameEvent_BotCrashUnique& event);
  [[nodiscard]] std::string toString() const;
  Team team; // the offending team that caused the crash
  std::optional<RobotID> violator;
  std::optional<RobotID> victim;
  std::optional<Vector2> crashPosition;
  std::optional<double> crashSpeed;
  std::optional<double> speedDifference;
  std::optional<double> crashAngle; //in radians
};
struct BotPushedBot{
  BotPushedBot(const proto::GameEvent_BotPushedBot& event);
  [[nodiscard]] std::string toString() const;
  Team team; // the offending team that caused the crash
  std::optional<RobotID> violator;
  std::optional<RobotID> victim;
  std::optional<Vector2> location;
  std::optional<double> distance;//total distance the robot was pushed
};
struct BotTippedOver{
  BotTippedOver(const proto::GameEvent_BotTippedOver& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  std::optional<RobotID> botID;
  std::optional<Vector2> location;
};
// Only is sent if robot touches ball
struct DefenderInDefenseArea{
  DefenderInDefenseArea(const proto::GameEvent_DefenderInDefenseArea& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  std::optional<RobotID> botID;
  std::optional<Vector2> location;
  std::optional<double> distance;//distance to nearest point outside defense area
};
// Only is sent if robot touches ball
struct DefenderInDefenseAreaPartially{
  DefenderInDefenseAreaPartially(const proto::GameEvent_DefenderInDefenseAreaPartially& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  std::optional<RobotID> botID;
  std::optional<Vector2> location;
  std::optional<double> distance;//distance with which the bot is in the defense area
};
struct AttackerTouchedBallInDefenseArea{
  AttackerTouchedBallInDefenseArea(const proto::GameEvent_AttackerTouchedBallInDefenseArea& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  std::optional<RobotID> botID;
  std::optional<Vector2> location;
  std::optional<double> distance;//distance with which the bot is in the defense area
};
struct BotKickedBallTooFast{
  BotKickedBallTooFast(const proto::GameEvent_BotKickedBallTooFast& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  bool chipped = false;
  std::optional<RobotID> botID;
  std::optional<Vector2> location;
  std::optional<double> kickSpeed;
};
struct BotDribbledBallTooFar{
  BotDribbledBallTooFar(const proto::GameEvent_BotDribbledBallTooFar& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  std::optional<RobotID> botID;
  std::optional<Vector2> startLocation; // location where dribbling started.
  std::optional<Vector2> endLocation; // location where ball was dribbled too far
};
struct AttackerTouchedOpponentInDefenseArea{
  AttackerTouchedOpponentInDefenseArea(const proto::GameEvent_AttackerTouchedOpponentInDefenseArea& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  std::optional<RobotID> botID;
  std::optional<Vector2> location;
  std::optional<RobotID> victim;
};
struct AttackerDoubleTouchedBall{
  AttackerDoubleTouchedBall(const proto::GameEvent_AttackerDoubleTouchedBall& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  std::optional<RobotID> botID;
  std::optional<Vector2> location;
};
struct AttackerTooCloseToDefenseArea{
  AttackerTooCloseToDefenseArea(const proto::GameEvent_AttackerTooCloseToDefenseArea& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  std::optional<RobotID> botID;
  std::optional<Vector2> location;
  std::optional<double> distance;//distance to the penalty area
};
struct BotHeldBallDeliberately{
  BotHeldBallDeliberately(const proto::GameEvent_BotHeldBallDeliberately& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  std::optional<RobotID> botID;
  std::optional<Vector2> location;
  std::optional<Time> timeHeld;
};
struct BotInterferedPlacement{
  BotInterferedPlacement(const proto::GameEvent_BotInterferedPlacement& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  std::optional<RobotID> botID;
  std::optional<Vector2> location;
};
struct MultipleCards{
  MultipleCards(const proto::GameEvent_MultipleCards& event);
  [[nodiscard]] std::string toString() const;
  Team team; //team that has gotten multiple cards
};
struct MultipleFouls{
  MultipleFouls(const proto::GameEvent_MultipleFouls& event);
  [[nodiscard]] std::string toString() const;
  Team team;
};
struct MultiplePlacementFailures{
  MultiplePlacementFailures(const proto::GameEvent_MultiplePlacementFailures& event);
  [[nodiscard]] std::string toString() const;
  Team team;
};
struct KickTimeout{
  KickTimeout(const proto::GameEvent_KickTimeout& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  std::optional<Vector2> location; //ball location
  std::optional<Time> timeWaited; //time waited for kick
};
struct NoProgressInGame{
  NoProgressInGame(const proto::GameEvent_NoProgressInGame& event);
  [[nodiscard]] std::string toString() const;
  std::optional<Vector2> location; // ball location
  std::optional<Time> timeWaited; // time for which there is no progress
};
struct PlacementFailed{
  PlacementFailed(const proto::GameEvent_PlacementFailed& event);
  [[nodiscard]] std::string toString() const;
  Team team; //team which should place the ball
  std::optional<double> distance;// distance from placement point
};
struct UnsportingBehaviorMinor{
  UnsportingBehaviorMinor(const proto::GameEvent_UnsportingBehaviorMinor& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  std::string reason;
};
struct UnsportingBehaviorMajor{
  UnsportingBehaviorMajor(const proto::GameEvent_UnsportingBehaviorMajor& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  std::string reason;
};
struct KeeperHeldBall{
  KeeperHeldBall(const proto::GameEvent_KeeperHeldBall& event);
  [[nodiscard]] std::string toString() const;
  Team team;
  std::optional<Vector2> location;
  std::optional<Time> duration;
};
struct PlacementSucceeded{
  PlacementSucceeded(const proto::GameEvent_PlacementSucceeded& event);
  [[nodiscard]] std::string toString() const;
  Team team; //team that placed the ball
  std::optional<Time> time;//Time it took to place the ball
  std::optional<double> precision; //distance to actual placement marker
  std::optional<double> initialDistance; // initial distance to the placement point when placement started
};
struct Prepared{
  Prepared(const proto::GameEvent_Prepared& event);
  [[nodiscard]] std::string toString() const;
  std::optional<Time> time;//Time it took for both teams to be prepared
};
struct BotSubstitution{
  BotSubstitution(const proto::GameEvent_BotSubstitution& event);
  [[nodiscard]] std::string toString() const;
  Team team;//team which requested a bot substitution
};
struct TooManyRobots{
  TooManyRobots(const proto::GameEvent_TooManyRobots& event);
  [[nodiscard]] std::string toString() const;
  Team team; //team which has too many robots
};
struct UnknownEvent{
  [[nodiscard]] std::string toString() const;
};
class GameEvent {
    public:
        GameEvent(const proto::GameEvent &event);
        enum Type{
          UNKNOWN_GAME_EVENT_TYPE = 0,

          PREPARED = 1,
          NO_PROGRESS_IN_GAME = 2,
          PLACEMENT_FAILED = 3,
          PLACEMENT_SUCCEEDED = 5,
          BOT_SUBSTITUTION = 37,
          TOO_MANY_ROBOTS = 38,
          BALL_LEFT_FIELD_TOUCH_LINE = 6,
          BALL_LEFT_FIELD_GOAL_LINE = 7,
          POSSIBLE_GOAL = 39,
          GOAL = 8,
          INDIRECT_GOAL = 9,
          CHIPPED_GOAL = 10,
          AIMLESS_KICK = 11,
          KICK_TIMEOUT = 12,
          KEEPER_HELD_BALL = 13,
          ATTACKER_DOUBLE_TOUCHED_BALL = 14,
          ATTACKER_TOUCHED_BALL_IN_DEFENSE_AREA = 15,
          ATTACKER_TOUCHED_OPPONENT_IN_DEFENSE_AREA = 16,
          ATTACKER_TOUCHED_OPPONENT_IN_DEFENSE_AREA_SKIPPED = 40,
          BOT_DRIBBLED_BALL_TOO_FAR = 17,
          BOT_KICKED_BALL_TOO_FAST = 18,
          ATTACKER_TOO_CLOSE_TO_DEFENSE_AREA = 19,
          BOT_INTERFERED_PLACEMENT = 20,
          BOT_CRASH_DRAWN = 21,
          BOT_CRASH_UNIQUE = 22,
          BOT_CRASH_UNIQUE_SKIPPED = 23,
          BOT_PUSHED_BOT = 24,
          BOT_PUSHED_BOT_SKIPPED = 25,
          BOT_HELD_BALL_DELIBERATELY = 26,
          BOT_TIPPED_OVER = 27,
          BOT_TOO_FAST_IN_STOP = 28,
          DEFENDER_TOO_CLOSE_TO_KICK_POINT = 29,
          DEFENDER_IN_DEFENSE_AREA_PARTIALLY = 30,
          DEFENDER_IN_DEFENSE_AREA = 31,
          MULTIPLE_CARDS = 32,
          MULTIPLE_PLACEMENT_FAILURES = 33,
          MULTIPLE_FOULS = 34,
          UNSPORTING_BEHAVIOR_MINOR = 35,
          UNSPORTING_BEHAVIOR_MAJOR = 36
        };
        [[nodiscard]] std::string toString() const;
        [[nodiscard]] bool isMatchProceeding() const;
        [[nodiscard]] bool isBallOutOfFieldEvent() const;
        [[nodiscard]] bool isFoul() const;
        [[nodiscard]] bool isOffense() const;
        [[nodiscard]] bool givesYellowCard() const;
        [[nodiscard]] bool givesRedCard() const;
    private:
        static Type getType(const proto::GameEvent &event);
        void setData(const proto::GameEvent &event);
        Type type;
        std::variant<UnknownEvent,BallLeftField,AimlessKick,
                     Goal,IndirectGoal,ChippedGoal,
                     BotTooFastInStop,DefenderTooCloseToKickPoint,BotCrashDrawn,
                     BotCrashUnique,BotPushedBot,BotTippedOver,
                     DefenderInDefenseArea, DefenderInDefenseAreaPartially, AttackerTouchedBallInDefenseArea,
                     BotKickedBallTooFast, BotDribbledBallTooFar, AttackerTouchedOpponentInDefenseArea,
                     AttackerDoubleTouchedBall, AttackerTooCloseToDefenseArea, BotHeldBallDeliberately,
                     BotInterferedPlacement, MultipleCards, MultipleFouls,
                     MultiplePlacementFailures, KickTimeout, NoProgressInGame,
                     PlacementFailed, UnsportingBehaviorMinor, UnsportingBehaviorMajor,
                     KeeperHeldBall, PlacementSucceeded, Prepared,
                     BotSubstitution, TooManyRobots> data;
};
#endif //SOCCER_GAMEEVENT_H
