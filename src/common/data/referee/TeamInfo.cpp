//
// Created by rolf on 29-03-20.
//

#include "TeamInfo.h"
unsigned int TeamInfo::activeYellowCards() const {
    return yellowCardTimes.size();
}
TeamInfo::TeamInfo(const proto::Referee::TeamInfo &teamInfo) {
    name = teamInfo.name();
    score = teamInfo.score();
    redCards = teamInfo.red_cards();
    for (const auto& yellowCard : teamInfo.yellow_card_times()){
        yellowCardTimes.emplace_back(Time((long)yellowCard*1000));
    }
    totalYellowCards = teamInfo.yellow_cards();
    timeOuts = teamInfo.timeouts();
    timeOutTime = Time((long)teamInfo.timeout_time()*1000);
    goalkeeperID = RobotID(teamInfo.goalkeeper());
    if(teamInfo.has_foul_counter()){
        foulCounter = teamInfo.foul_counter();
    }
    if(teamInfo.has_ball_placement_failures()){
        ballPlacementFailures = teamInfo.ball_placement_failures();
    }
    if(teamInfo.has_can_place_ball()){
        canPlaceBall = teamInfo.can_place_ball();
    }
    if(teamInfo.has_max_allowed_bots()){
        maxAllowedBots = teamInfo.max_allowed_bots();
    }else{
        maxAllowedBots = 11 - activeYellowCards() - redCards; //TODO: fix hardcoded 11 robots here?
    }
    if (teamInfo.has_bot_substitution_intent()){
        botSubstitutionIntent = teamInfo.bot_substitution_intent();
    }
    if (teamInfo.has_ball_placement_failures_reached()){
        ballPlacementFailuresReached = teamInfo.ball_placement_failures_reached();
    }
}
