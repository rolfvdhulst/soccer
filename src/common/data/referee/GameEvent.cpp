//
// Created by rolf on 28-03-20.
//

#include <geometry/Angle.h>
#include "GameEvent.h"
GameEvent::GameEvent(const proto::GameEvent &event) {
    type=getType(event);
    data = UnknownEvent();
    setData(event);
}
GameEvent::Type GameEvent::getType(const proto::GameEvent &event) {
    switch(event.type()){
    case proto::UNKNOWN_GAME_EVENT_TYPE: return UNKNOWN_GAME_EVENT_TYPE;
    case proto::PREPARED: return PREPARED;
    case proto::NO_PROGRESS_IN_GAME: return NO_PROGRESS_IN_GAME;
    case proto::PLACEMENT_FAILED: return PLACEMENT_FAILED;
    case proto::PLACEMENT_SUCCEEDED: return PLACEMENT_SUCCEEDED;
    case proto::BOT_SUBSTITUTION: return BOT_SUBSTITUTION;
    case proto::TOO_MANY_ROBOTS: return TOO_MANY_ROBOTS;
    case proto::BALL_LEFT_FIELD_TOUCH_LINE: return BALL_LEFT_FIELD_TOUCH_LINE;
    case proto::BALL_LEFT_FIELD_GOAL_LINE: return BALL_LEFT_FIELD_GOAL_LINE;
    case proto::POSSIBLE_GOAL: return POSSIBLE_GOAL;
    case proto::GOAL: return GOAL;
    case proto::INDIRECT_GOAL: return INDIRECT_GOAL;
    case proto::CHIPPED_GOAL: return CHIPPED_GOAL;
    case proto::AIMLESS_KICK: return AIMLESS_KICK;
    case proto::KICK_TIMEOUT: return KICK_TIMEOUT;
    case proto::KEEPER_HELD_BALL: return KEEPER_HELD_BALL;
    case proto::ATTACKER_DOUBLE_TOUCHED_BALL: return ATTACKER_DOUBLE_TOUCHED_BALL;
    case proto::ATTACKER_TOUCHED_BALL_IN_DEFENSE_AREA: return ATTACKER_TOUCHED_BALL_IN_DEFENSE_AREA;
    case proto::ATTACKER_TOUCHED_OPPONENT_IN_DEFENSE_AREA: return ATTACKER_TOUCHED_OPPONENT_IN_DEFENSE_AREA;
    case proto::ATTACKER_TOUCHED_OPPONENT_IN_DEFENSE_AREA_SKIPPED: return ATTACKER_TOUCHED_OPPONENT_IN_DEFENSE_AREA_SKIPPED;
    case proto::BOT_DRIBBLED_BALL_TOO_FAR: return BOT_DRIBBLED_BALL_TOO_FAR;
    case proto::BOT_KICKED_BALL_TOO_FAST: return BOT_KICKED_BALL_TOO_FAST;
    case proto::ATTACKER_TOO_CLOSE_TO_DEFENSE_AREA: return ATTACKER_TOO_CLOSE_TO_DEFENSE_AREA;
    case proto::BOT_INTERFERED_PLACEMENT: return BOT_INTERFERED_PLACEMENT;
    case proto::BOT_CRASH_DRAWN: return BOT_CRASH_DRAWN;
    case proto::BOT_CRASH_UNIQUE: return BOT_CRASH_UNIQUE;
    case proto::BOT_CRASH_UNIQUE_SKIPPED: return BOT_CRASH_UNIQUE_SKIPPED;
    case proto::BOT_PUSHED_BOT: return BOT_PUSHED_BOT;
    case proto::BOT_PUSHED_BOT_SKIPPED: return BOT_PUSHED_BOT_SKIPPED;
    case proto::BOT_HELD_BALL_DELIBERATELY: return BOT_HELD_BALL_DELIBERATELY;
    case proto::BOT_TIPPED_OVER: return BOT_TIPPED_OVER;
    case proto::BOT_TOO_FAST_IN_STOP: return BOT_TOO_FAST_IN_STOP;
    case proto::DEFENDER_TOO_CLOSE_TO_KICK_POINT: return DEFENDER_TOO_CLOSE_TO_KICK_POINT;
    case proto::DEFENDER_IN_DEFENSE_AREA_PARTIALLY: return DEFENDER_IN_DEFENSE_AREA_PARTIALLY;
    case proto::DEFENDER_IN_DEFENSE_AREA: return DEFENDER_IN_DEFENSE_AREA;
    case proto::MULTIPLE_CARDS: return MULTIPLE_CARDS;
    case proto::MULTIPLE_PLACEMENT_FAILURES: return MULTIPLE_PLACEMENT_FAILURES;
    case proto::MULTIPLE_FOULS: return MULTIPLE_FOULS;
    case proto::UNSPORTING_BEHAVIOR_MINOR: return UNSPORTING_BEHAVIOR_MINOR;
    case proto::UNSPORTING_BEHAVIOR_MAJOR: return UNSPORTING_BEHAVIOR_MAJOR;
    }
}
void GameEvent::setData(const proto::GameEvent &event) {

    switch(event.event_case()){
    case proto::GameEvent::kPrepared:
        data = Prepared(event.prepared());
        return;
    case proto::GameEvent::kNoProgressInGame:
        data = NoProgressInGame(event.no_progress_in_game());
        return;
    case proto::GameEvent::kPlacementFailed:
        data = PlacementFailed(event.placement_failed());
        return;
    case proto::GameEvent::kPlacementSucceeded:
        data = PlacementSucceeded(event.placement_succeeded());
        return;
    case proto::GameEvent::kBotSubstitution:
        data = BotSubstitution(event.bot_substitution());
        return;
    case proto::GameEvent::kTooManyRobots:
        data = TooManyRobots(event.too_many_robots());
        return;
    case proto::GameEvent::kBallLeftFieldTouchLine:
        data = BallLeftField(event.ball_left_field_touch_line(),false);
        return;
    case proto::GameEvent::kBallLeftFieldGoalLine:
        data = BallLeftField(event.ball_left_field_touch_line(),true);
        return;
    case proto::GameEvent::kPossibleGoal:
        data = Goal(event.possible_goal());
        return;
    case proto::GameEvent::kGoal:
        data=Goal(event.goal());
        return;
    case proto::GameEvent::kIndirectGoal:
        data =IndirectGoal(event.indirect_goal());
        return;
    case proto::GameEvent::kChippedGoal:
        data=ChippedGoal(event.chipped_goal());
        return;
    case proto::GameEvent::kAimlessKick:
        data=AimlessKick(event.aimless_kick());
        return;
    case proto::GameEvent::kKickTimeout:
        data=KickTimeout(event.kick_timeout());
        return;
    case proto::GameEvent::kKeeperHeldBall:
        data=KeeperHeldBall(event.keeper_held_ball());
        return;
    case proto::GameEvent::kAttackerDoubleTouchedBall:
        data=AttackerDoubleTouchedBall(event.attacker_double_touched_ball());
        return;
    case proto::GameEvent::kAttackerTouchedBallInDefenseArea:
        data=AttackerTouchedBallInDefenseArea(event.attacker_touched_ball_in_defense_area());
        return;
    case proto::GameEvent::kAttackerTouchedOpponentInDefenseArea:
        data=AttackerTouchedOpponentInDefenseArea(event.attacker_touched_opponent_in_defense_area());
        return;
    case proto::GameEvent::kAttackerTouchedOpponentInDefenseAreaSkipped:
        data=AttackerTouchedOpponentInDefenseArea(event.attacker_touched_opponent_in_defense_area());
        return;
    case proto::GameEvent::kBotDribbledBallTooFar:
        data=BotDribbledBallTooFar(event.bot_dribbled_ball_too_far());
        return;
    case proto::GameEvent::kBotKickedBallTooFast:
        data=BotKickedBallTooFast(event.bot_kicked_ball_too_fast());
        return;
    case proto::GameEvent::kAttackerTooCloseToDefenseArea:
        data = AttackerTooCloseToDefenseArea(event.attacker_too_close_to_defense_area());
        return;
    case proto::GameEvent::kBotInterferedPlacement:
        data = BotInterferedPlacement(event.bot_interfered_placement());
        return;
    case proto::GameEvent::kBotCrashDrawn:
        data = BotCrashDrawn(event.bot_crash_drawn());
        return;
    case proto::GameEvent::kBotCrashUnique:
        data = BotCrashUnique(event.bot_crash_unique());
        return;
    case proto::GameEvent::kBotCrashUniqueSkipped:
        data = BotCrashUnique(event.bot_crash_unique());
        return;
    case proto::GameEvent::kBotPushedBot:
        data = BotPushedBot(event.bot_pushed_bot());
        return;
    case proto::GameEvent::kBotPushedBotSkipped:
        data = BotPushedBot(event.bot_pushed_bot());
        return;
    case proto::GameEvent::kBotHeldBallDeliberately:
        data = BotHeldBallDeliberately(event.bot_held_ball_deliberately());
        return;
    case proto::GameEvent::kBotTippedOver:
        data = BotTippedOver(event.bot_tipped_over());
        return;
    case proto::GameEvent::kBotTooFastInStop:
        data = BotTooFastInStop(event.bot_too_fast_in_stop());
        return;
    case proto::GameEvent::kDefenderTooCloseToKickPoint:
        data = DefenderTooCloseToKickPoint(event.defender_too_close_to_kick_point());
        return;
    case proto::GameEvent::kDefenderInDefenseAreaPartially:
        data = DefenderInDefenseAreaPartially(event.defender_in_defense_area_partially());
        return;
    case proto::GameEvent::kDefenderInDefenseArea:
        data = DefenderInDefenseArea(event.defender_in_defense_area());
        return;
    case proto::GameEvent::kMultipleCards:
        data = MultipleCards(event.multiple_cards());
        return;
    case proto::GameEvent::kMultiplePlacementFailures:
        data = MultiplePlacementFailures(event.multiple_placement_failures());
        return;
    case proto::GameEvent::kMultipleFouls:
        data = MultipleFouls(event.multiple_fouls());
        return;
    case proto::GameEvent::kUnsportingBehaviorMinor:
        data = UnsportingBehaviorMinor(event.unsporting_behavior_minor());
        return;
    case proto::GameEvent::kUnsportingBehaviorMajor:
        data = UnsportingBehaviorMajor(event.unsporting_behavior_major());
        return;
    case proto::GameEvent::EVENT_NOT_SET: return;
    }
}
std::string GameEvent::toString() const {
    switch(type){

    case PREPARED:return std::get<Prepared>(data).toString();
    case NO_PROGRESS_IN_GAME:return std::get<NoProgressInGame>(data).toString();
    case PLACEMENT_FAILED:return std::get<PlacementFailed>(data).toString();
    case PLACEMENT_SUCCEEDED:return std::get<PlacementSucceeded>(data).toString();
    case BOT_SUBSTITUTION:return std::get<BotSubstitution>(data).toString();
    case TOO_MANY_ROBOTS:return std::get<TooManyRobots>(data).toString();
    case BALL_LEFT_FIELD_TOUCH_LINE:
    case BALL_LEFT_FIELD_GOAL_LINE:return std::get<BallLeftField>(data).toString();
    case POSSIBLE_GOAL:
    case GOAL:return std::get<Goal>(data).toString();
    case INDIRECT_GOAL:return std::get<IndirectGoal>(data).toString();
    case CHIPPED_GOAL:return std::get<ChippedGoal>(data).toString();
    case AIMLESS_KICK:return std::get<AimlessKick>(data).toString();
    case KICK_TIMEOUT:return std::get<KickTimeout>(data).toString();
    case KEEPER_HELD_BALL:return std::get<KeeperHeldBall>(data).toString();
    case ATTACKER_DOUBLE_TOUCHED_BALL:return std::get<AttackerDoubleTouchedBall>(data).toString();
    case ATTACKER_TOUCHED_BALL_IN_DEFENSE_AREA:return std::get<AttackerTouchedBallInDefenseArea>(data).toString();
    case ATTACKER_TOUCHED_OPPONENT_IN_DEFENSE_AREA:
    case ATTACKER_TOUCHED_OPPONENT_IN_DEFENSE_AREA_SKIPPED:return std::get<AttackerTouchedOpponentInDefenseArea>(data).toString();
    case BOT_DRIBBLED_BALL_TOO_FAR:return std::get<BotDribbledBallTooFar>(data).toString();
    case BOT_KICKED_BALL_TOO_FAST:return std::get<BotKickedBallTooFast>(data).toString();
    case ATTACKER_TOO_CLOSE_TO_DEFENSE_AREA:return std::get<AttackerTooCloseToDefenseArea>(data).toString();
    case BOT_INTERFERED_PLACEMENT:return std::get<BotInterferedPlacement>(data).toString();
    case BOT_CRASH_DRAWN:return std::get<BotCrashDrawn>(data).toString();
    case BOT_CRASH_UNIQUE:
    case BOT_CRASH_UNIQUE_SKIPPED:return std::get<BotCrashUnique>(data).toString();
    case BOT_PUSHED_BOT:
    case BOT_PUSHED_BOT_SKIPPED:return std::get<BotPushedBot>(data).toString();
    case BOT_HELD_BALL_DELIBERATELY:return std::get<BotHeldBallDeliberately>(data).toString();
    case BOT_TIPPED_OVER:return std::get<BotTippedOver>(data).toString();
    case BOT_TOO_FAST_IN_STOP:return std::get<BotTooFastInStop>(data).toString();
    case DEFENDER_TOO_CLOSE_TO_KICK_POINT:return std::get<DefenderTooCloseToKickPoint>(data).toString();
    case DEFENDER_IN_DEFENSE_AREA_PARTIALLY:return std::get<DefenderInDefenseAreaPartially>(data).toString();
    case DEFENDER_IN_DEFENSE_AREA:return std::get<DefenderInDefenseArea>(data).toString();
    case MULTIPLE_CARDS:return std::get<MultipleCards>(data).toString();
    case MULTIPLE_PLACEMENT_FAILURES:return std::get<MultiplePlacementFailures>(data).toString();
    case MULTIPLE_FOULS:return std::get<MultipleFouls>(data).toString();
    case UNSPORTING_BEHAVIOR_MINOR:return std::get<UnsportingBehaviorMinor>(data).toString();
    case UNSPORTING_BEHAVIOR_MAJOR:return std::get<UnsportingBehaviorMajor>(data).toString();
    case UNKNOWN_GAME_EVENT_TYPE:return std::get<UnknownEvent>(data).toString();
    }
}
bool GameEvent::isMatchProceeding() const {
    return type == PREPARED ||
    type == NO_PROGRESS_IN_GAME ||
    type == PLACEMENT_FAILED ||
    type == PLACEMENT_SUCCEEDED ||
    type == BOT_SUBSTITUTION ||
    type == TOO_MANY_ROBOTS;
}
bool GameEvent::isBallOutOfFieldEvent() const {
    return type == BALL_LEFT_FIELD_GOAL_LINE ||
    type == BALL_LEFT_FIELD_TOUCH_LINE ||
    type == GOAL ||
    type == POSSIBLE_GOAL ||
    type == INDIRECT_GOAL ||
    type == CHIPPED_GOAL;
}
bool GameEvent::isFoul() const {
    return  type == ATTACKER_TOO_CLOSE_TO_DEFENSE_AREA ||
    type == BOT_INTERFERED_PLACEMENT ||
    type == BOT_CRASH_DRAWN ||
    type == BOT_CRASH_UNIQUE ||
    type == BOT_CRASH_UNIQUE_SKIPPED ||
    type == BOT_PUSHED_BOT ||
    type == BOT_PUSHED_BOT_SKIPPED ||
    type == BOT_HELD_BALL_DELIBERATELY ||
    type == BOT_TIPPED_OVER ||
    type == BOT_TOO_FAST_IN_STOP ||
    type == DEFENDER_TOO_CLOSE_TO_KICK_POINT ||
    type == DEFENDER_IN_DEFENSE_AREA_PARTIALLY ||
    type == DEFENDER_IN_DEFENSE_AREA;
}
bool GameEvent::isOffense() const{
    return type == NO_PROGRESS_IN_GAME ||
    type == ATTACKER_DOUBLE_TOUCHED_BALL ||
    type == UNSPORTING_BEHAVIOR_MINOR ||
    type == UNSPORTING_BEHAVIOR_MAJOR ||
    type == MULTIPLE_FOULS ||
    type == MULTIPLE_CARDS;
}
bool GameEvent::givesYellowCard() const {
    return type == MULTIPLE_FOULS; //TODO: check other offenses/rules?
}
bool GameEvent::givesRedCard() const {
    return type == MULTIPLE_CARDS; //new rules 
}
Goal::Goal(const proto::GameEvent_Goal &event){
    team = Team(event.by_team());
    if (event.has_kicking_team()){
        kickingTeam = Team(event.kicking_team());
    }
    if (event.has_kicking_bot()){
        botID = RobotID(event.kicking_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_kick_location()){
        kickLocation = Vector2(event.kick_location());
    }

}
std::string Goal::toString() const {
    std::string base = "Goal for " +team.toString();
    std::string byTeam = kickingTeam?  " by " + kickingTeam->toString() :"";
    std::string byBot = botID? " bot " + botID->toString() : "";
    std::string at = location ? " at x: " + std::to_string(location->x)+", y: " + std::to_string(location->y):"";
    std::string from = kickLocation ? " from x: " + std::to_string(kickLocation->x)+", y: " + std::to_string(kickLocation->y):"";
    return base+byTeam+byBot+at+from;
}
BallLeftField::BallLeftField(const proto::GameEvent_BallLeftField &event, bool atGoalLine) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    throughGoalLine = atGoalLine;
}
std::string BallLeftField::toString() const {
    std::string base="BallLeftField through ";
    std::string line = throughGoalLine? "goal line" : "touch line";
    std::string next = " by " + team.toString();
    std::string id = botID ? " bot "+(*botID).toString() : "";
    std::string at = location ? " at x: " + std::to_string(location->x)+", y: " + std::to_string(location->y):"";
    return base+line+next+id+at;
}
AimlessKick::AimlessKick(const proto::GameEvent_AimlessKick &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_kick_location()){
        kickLocation = Vector2(event.kick_location());
    }
}
std::string AimlessKick::toString() const {
    std::string base="AimlessKick by "+ team.toString();
    std::string id = botID ? " bot "+(*botID).toString() : "";
    std::string at = location ? " at x: " + std::to_string(location->x)+", y: " + std::to_string(location->y):"";
    std::string from = kickLocation ? " from x: " + std::to_string(kickLocation->x)+", y: " + std::to_string(kickLocation->y):"";
    return base+id+at+from;
}
IndirectGoal::IndirectGoal(const proto::GameEvent_IndirectGoal &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_kick_location()){
        kickLocation = Vector2(event.kick_location());
    }
}
std::string IndirectGoal::toString() const {
    std::string base = "IndirectGoal by " +team.toString();
    std::string byBot = botID? " bot " + botID->toString() : "";
    std::string at = location ? " at x: " + std::to_string(location->x)+", y: " + std::to_string(location->y):"";
    std::string from = kickLocation ? " from x: " + std::to_string(kickLocation->x)+", y: " + std::to_string(kickLocation->y):"";
    return base+byBot+at+from;
}
ChippedGoal::ChippedGoal(const proto::GameEvent_ChippedGoal &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_kick_location()){
        kickLocation = Vector2(event.kick_location());
    }
    if (event.has_max_ball_height()){
        ballHeight = event.max_ball_height();
    }
}
std::string ChippedGoal::toString() const {
    std::string base = "ChippedGoal by " +team.toString();
    std::string byBot = botID? " bot " + botID->toString() : "";
    std::string height = ballHeight ? " with ballHeight " + std::to_string(*ballHeight) : "";
    std::string at = location ? " at x: " + std::to_string(location->x)+", y: " + std::to_string(location->y):"";
    std::string from = kickLocation ? " from x: " + std::to_string(kickLocation->x)+", y: " + std::to_string(kickLocation->y):"";

    return base+byBot+height+at+from;
}
BotTooFastInStop::BotTooFastInStop(const proto::GameEvent_BotTooFastInStop &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_speed()){
        speed = event.speed();
    }
}
std::string BotTooFastInStop::toString() const {
    std::string base = "BotTooFastInStop by " +team.toString();
    std::string byBot = botID? " bot " + botID->toString() : "";
    std::string at = location ? " at x: " + std::to_string(location->x)+", y: " + std::to_string(location->y):"";
    std::string withSpeed = speed ? " with " +std::to_string(*speed) + " m/s" : "";
    return base+byBot+at+withSpeed;
}
DefenderTooCloseToKickPoint::DefenderTooCloseToKickPoint(const proto::GameEvent_DefenderTooCloseToKickPoint &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_distance()){
        distance = event.distance();
    }

}
std::string DefenderTooCloseToKickPoint::toString() const {
    std::string base = "DefenderTooCloseToKickPoint by " +team.toString();
    std::string byBot = botID? " bot " + botID->toString() : "";
    std::string with = distance ? " with " + std::to_string(*distance)+ " m" : "";
    return base+byBot+with;
}
BotCrashDrawn::BotCrashDrawn(const proto::GameEvent_BotCrashDrawn &event) {
    if (event.has_bot_yellow()){
        yellowID = RobotID(event.bot_yellow());
    }
    if (event.has_bot_blue()){
        blueID = RobotID(event.bot_blue());
    }
    if (event.has_location()){
        crashPosition = Vector2(event.location());
    }
    if (event.has_speed_diff()){
        speedDifference = event.speed_diff();
    }
    if (event.has_crash_speed()){
        crashSpeed = event.crash_speed();
    }
    if (event.has_crash_angle()){
        crashAngle = event.crash_angle();
    }
}
std::string BotCrashDrawn::toString() const {
    std::string base = "BotCrashDrawn between yellow";
    std::string yellow = yellowID? " bot " + yellowID->toString() : "";
    std::string blue = blueID? " and blue bot " + blueID->toString() : "";
    std::string deltaSpeed = speedDifference? " with deltaV: " +std::to_string(*speedDifference) +"m/s": "";
    std::string angle = crashAngle? " at " + std::to_string(toDegrees(*crashAngle)) +" degrees": "";
    return base+yellow+blue+deltaSpeed+angle;
}
BotCrashUnique::BotCrashUnique(const proto::GameEvent_BotCrashUnique &event) {
    team = Team(event.by_team());
    if (event.has_violator()){
        violator = RobotID(event.violator());
    }
    if (event.has_victim()){
        victim = RobotID(event.victim());
    }
    if (event.has_location()){
        crashPosition = Vector2(event.location());
    }
    if (event.has_speed_diff()){
        speedDifference = event.speed_diff();
    }
    if (event.has_crash_speed()){
        crashSpeed = event.crash_speed();
    }
    if (event.has_crash_angle()){
        crashAngle = event.crash_angle();
    }

}
std::string BotCrashUnique::toString() const {
    std::string base = "BotCrashUnique by " +team.toString();
    std::string offender = violator? " bot " + violator->toString() : "";
    std::string blue = victim? "  with bot " + victim->toString() : "";
    std::string absSpeed = crashSpeed? " with " + std::to_string(*crashSpeed) + " m/s" : "";
    std::string deltaSpeed = speedDifference? " with deltaV: " +std::to_string(*speedDifference) + " m/s": "";
    std::string angle = crashAngle? " at " + std::to_string(toDegrees(*crashAngle)) +" degrees" : "";
    return base+offender+blue+deltaSpeed+angle;
}
BotPushedBot::BotPushedBot(const proto::GameEvent_BotPushedBot &event) {
    team = Team(event.by_team());
    if (event.has_violator()){
        violator = RobotID(event.violator());
    }
    if (event.has_victim()){
        victim = RobotID(event.victim());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_pushed_distance()){
        distance = event.pushed_distance();
    }
}
std::string BotPushedBot::toString() const {
    std::string base = "BotPushedBot by " +team.toString();
    std::string offender = violator? " bot " + violator->toString() : "";
    std::string blue = victim? " to bot " + victim->toString() : "";
    std::string dist = distance? " for " +std::to_string(*distance) + " m" : "";
    return base+offender+blue+dist;
}
BotTippedOver::BotTippedOver(const proto::GameEvent_BotTippedOver &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
}
std::string BotTippedOver::toString() const {
    std::string base = "BotTippedOver by " +team.toString();
    std::string offender = botID? " bot " + botID->toString() : "";
    std::string at = location ? " at x: " + std::to_string(location->x)+", y: " + std::to_string(location->y):"";
    return base+offender+at;
}
DefenderInDefenseArea::DefenderInDefenseArea(const proto::GameEvent_DefenderInDefenseArea &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_distance()){
        distance = event.distance();
    }
}
std::string DefenderInDefenseArea::toString() const {
    std::string base = "DefenderInDefenseArea by " +team.toString();
    std::string offender = botID? " bot " + botID->toString() : "";
    std::string dist = distance ? " with " + std::to_string(*distance) +" m":"";
    return base+offender+dist;
}
DefenderInDefenseAreaPartially::DefenderInDefenseAreaPartially(
        const proto::GameEvent_DefenderInDefenseAreaPartially &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_distance()){
        distance = event.distance();
    }
}
std::string DefenderInDefenseAreaPartially::toString() const {
    std::string base = "DefenderInDefenseAreaPartially by " +team.toString();
    std::string offender = botID? " bot " + botID->toString() : "";
    std::string dist = distance ? " with " + std::to_string(*distance) +" m":"";
    return base+offender+dist;
}
AttackerTouchedBallInDefenseArea::AttackerTouchedBallInDefenseArea(
        const proto::GameEvent_AttackerTouchedBallInDefenseArea &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_distance()){
        distance = event.distance();
    }
}
std::string AttackerTouchedBallInDefenseArea::toString() const {
    std::string base = "AttackerTouchedBallInDefenseArea by " +team.toString();
    std::string offender = botID? " bot " + botID->toString() : "";
    std::string dist = distance ? " with " + std::to_string(*distance) +" m":"";
    return base+offender+dist;
}
BotKickedBallTooFast::BotKickedBallTooFast(const proto::GameEvent_BotKickedBallTooFast &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_initial_ball_speed()){
        kickSpeed = event.initial_ball_speed();
    }
    if (event.has_chipped()){
        chipped = event.chipped();
    }
}
std::string BotKickedBallTooFast::toString() const {
    std::string base = "BotKickedBallTooFast by " +team.toString();
    std::string offender = botID? " bot " + botID->toString() : "";
    std::string kick = chipped ? " chipped" : " kicked";
    std::string speed = kickSpeed? " with " + std::to_string(*kickSpeed) +" m/s":"";
    return base+offender+kick+speed;
}
BotDribbledBallTooFar::BotDribbledBallTooFar(const proto::GameEvent_BotDribbledBallTooFar &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_start()){
        startLocation = Vector2(event.start());
    }
    if (event.has_end()){
        endLocation = Vector2(event.end());
    }
}
std::string BotDribbledBallTooFar::toString() const {
    std::string base = "BotDribbledBallTooFar by " +team.toString();
    std::string offender = botID? " bot " + botID->toString() : "";
    std::string distance = (startLocation&&endLocation) ? " with " +std::to_string((*startLocation-*endLocation).length())+" m":"";
    return base+offender+distance;
}
AttackerTouchedOpponentInDefenseArea::AttackerTouchedOpponentInDefenseArea(
        const proto::GameEvent_AttackerTouchedOpponentInDefenseArea &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_victim()){
        victim = RobotID(event.victim());
    }
}
std::string AttackerTouchedOpponentInDefenseArea::toString() const {
    std::string base = "AttackerTouchedOpponentInDefenseArea by " +team.toString();
    std::string offender = botID? " bot " + botID->toString() : "";
    std::string vic = victim? "  with bot " + victim->toString() : "";
    return base+offender+vic;
}
AttackerDoubleTouchedBall::AttackerDoubleTouchedBall(const proto::GameEvent_AttackerDoubleTouchedBall &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
}
std::string AttackerDoubleTouchedBall::toString() const {
    std::string base = "AttackerDoubleTouchedBall by " +team.toString();
    std::string offender = botID? " bot " + botID->toString() : "";
    return base+offender;
}
AttackerTooCloseToDefenseArea::AttackerTooCloseToDefenseArea(
        const proto::GameEvent_AttackerTooCloseToDefenseArea &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_distance()){
        distance = event.distance();
    }
}
std::string AttackerTooCloseToDefenseArea::toString() const {
    std::string base = "AttackerTooCloseToDefenseArea by " +team.toString();
    std::string offender = botID? " bot " + botID->toString() : "";
    std::string dist = distance ? " with " + std::to_string(*distance) +" m to the Defense Area":"";
    return base+offender+dist;
}
BotHeldBallDeliberately::BotHeldBallDeliberately(const proto::GameEvent_BotHeldBallDeliberately &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_duration()){
        timeHeld = Time(event.duration());
    }
}
std::string BotHeldBallDeliberately::toString() const {
    std::string base = "BotHeldBallDeliberately by " +team.toString();
    std::string offender = botID? " bot " + botID->toString() : "";
    std::string duration = timeHeld? " for "+ std::to_string(timeHeld->asSeconds()) +" s" :"";
    return base+offender+duration;
}
BotInterferedPlacement::BotInterferedPlacement(const proto::GameEvent_BotInterferedPlacement &event) {
    team = Team(event.by_team());
    if (event.has_by_bot()){
        botID = RobotID(event.by_bot());
    }
    if (event.has_location()){
        location = Vector2(event.location());
    }
}
std::string BotInterferedPlacement::toString() const {
    std::string base = "BotInterferedPlacement by " +team.toString();
    std::string offender = botID? " bot " + botID->toString() : "";
    return base+offender;
}
MultipleCards::MultipleCards(const proto::GameEvent_MultipleCards &event) {
    team = Team(event.by_team());
}
std::string MultipleCards::toString() const {
    return "MultipleCards for "+team.toString();
}
MultipleFouls::MultipleFouls(const proto::GameEvent_MultipleFouls &event) {
    team = Team(event.by_team());
}
std::string MultipleFouls::toString() const {
    return "MultipleFouls for "+team.toString();
}
MultiplePlacementFailures::MultiplePlacementFailures(const proto::GameEvent_MultiplePlacementFailures &event) {
    team = Team(event.by_team());
}
std::string MultiplePlacementFailures::toString() const {
    return "MultiplePlacementFailures for "+team.toString();
}
KickTimeout::KickTimeout(const proto::GameEvent_KickTimeout &event) {
    team = Team(event.by_team());
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_time()){
        timeWaited = Time(event.time());
    }
}
std::string KickTimeout::toString() const {
    std::string base = "KickTimeout for " +team.toString();
    std::string duration = timeWaited? " after "+std::to_string(timeWaited->asSeconds()) + " s": "";
    return base + duration;
}
NoProgressInGame::NoProgressInGame(const proto::GameEvent_NoProgressInGame &event) {
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_time()){
        timeWaited = Time(event.time());
    }
}
std::string NoProgressInGame::toString() const {
    std::string base = "NoProgressInGame";
    std::string duration = timeWaited? " after "+std::to_string(timeWaited->asSeconds()) + " s": "";
    return base+duration;
}
PlacementFailed::PlacementFailed(const proto::GameEvent_PlacementFailed &event) {
    team = Team(event.by_team());
    if (event.has_remaining_distance()){
        distance = event.remaining_distance();
    }
}
std::string PlacementFailed::toString() const {
    std::string base = "PlacementFailed by " +team.toString();
    std::string dist= distance ?  " with " + std::to_string(*distance) + " m" : "";
    return base+dist;
}
UnsportingBehaviorMinor::UnsportingBehaviorMinor(const proto::GameEvent_UnsportingBehaviorMinor &event) {
    team = Team(event.by_team());
    reason = event.reason();
}
std::string UnsportingBehaviorMinor::toString() const {
    std::string base = "UnsportingBehaviorMinor by " +team.toString();
    std::string argument = ", reason: " + reason;
    return base+argument;
}
UnsportingBehaviorMajor::UnsportingBehaviorMajor(const proto::GameEvent_UnsportingBehaviorMajor &event) {
    team = Team(event.by_team());
    reason = event.reason();
}
std::string UnsportingBehaviorMajor::toString() const {
    std::string base = "UnsportingBehaviorMajor by " +team.toString();
    std::string argument = ", reason: " + reason;
    return base+argument;
}
KeeperHeldBall::KeeperHeldBall(const proto::GameEvent_KeeperHeldBall &event) {
    team = Team(event.by_team());
    if (event.has_location()){
        location = Vector2(event.location());
    }
    if (event.has_duration()){
        duration = Time(event.duration());
    }
}
std::string KeeperHeldBall::toString() const {
    std::string base = "KeeperHeldBall by " +team.toString();
    std::string time = duration? " for "+std::to_string(duration->asSeconds()) + " s": "";
    return base+time;
}
PlacementSucceeded::PlacementSucceeded(const proto::GameEvent_PlacementSucceeded &event) {
    team = Team(event.by_team());
    if (event.has_distance()){
        initialDistance = event.distance();
    }
    if (event.has_precision()){
        precision = event.precision();
    }
    if (event.has_time_taken()){
        time = Time(event.time_taken());
    }
}
std::string PlacementSucceeded::toString() const {
    std::string base = "PlacementSucceeded by " +team.toString();
    std::string duration = time? " after "+std::to_string(time->asSeconds()) + " s": "";
    std::string distance = precision? " within "+ std::to_string(*precision) + " m": "";
    return base+duration+distance;
}
Prepared::Prepared(const proto::GameEvent_Prepared &event) {
    if (event.has_time_taken()){
        time = Time(event.time_taken());
    }
}
std::string Prepared::toString() const {
    std::string base = "Prepared";
    std::string duration = time? " after "+std::to_string(time->asSeconds()) + " s": "";
    return base+duration;
}
BotSubstitution::BotSubstitution(const proto::GameEvent_BotSubstitution &event) {
    team = Team(event.by_team());
}
std::string BotSubstitution::toString() const {
    return "BotSubstitution requested by "+team.toString();
}
TooManyRobots::TooManyRobots(const proto::GameEvent_TooManyRobots &event) {
    team = Team(event.by_team());
}
std::string TooManyRobots::toString() const {
    return "TooManyRobots for "+team.toString();
}
std::string UnknownEvent::toString() const {
    return "UnknownEvent";
}
