//
// Created by rolf on 29-03-20.
//

#include <protobuf/World.pb.h>
#include "RefereeFilter.h"
proto::GameState RefereeFilter::update(const std::vector<proto::Referee>& refereeMessages, const proto::World& world) {
    flipChanged = false;
    if(refereeMessages.empty()){
        return lastGameState;
    }
    //Note the below line assumes that the referee messages are sorted ascending by time!
    const proto::Referee lastRefMessage=refereeMessages.back();
    bool weAreBlue=inferOurColor(lastRefMessage);
    proto::GameState newGameState = createGameState(lastRefMessage,weAreBlue);

    lastGameState = newGameState;
    return newGameState;
}
proto::GameState RefereeFilter::createGameState(const proto::Referee &lastRefMessage,bool weAreBlue) {
    proto::GameState newGameState;
    newGameState.set_timestamp(lastRefMessage.packet_timestamp()*1000);
    newGameState.set_stage(lastRefMessage.stage());
    if(lastRefMessage.has_stage_time_left()){
        newGameState.set_stage_time_left((long)lastRefMessage.stage_time_left()*1000);
    }
    //actual command is added in addCommands() function

    newGameState.set_command_counter(lastRefMessage.command_counter());
    newGameState.set_command_timestamp(lastRefMessage.command_timestamp()*1000);

    if (weAreBlue){
        newGameState.mutable_us()->CopyFrom(lastRefMessage.blue());
        newGameState.mutable_them()->CopyFrom(lastRefMessage.yellow());
    }else{
        newGameState.mutable_us()->CopyFrom(lastRefMessage.yellow());
        newGameState.mutable_them()->CopyFrom(lastRefMessage.blue());
    }
    bool flipField = ! weAreBlue;
    if(lastRefMessage.has_blue_team_on_positive_half()){
        flipField = weAreBlue == lastRefMessage.blue_team_on_positive_half();
    }
    newGameState.set_weplayonpositivehalf(flipField);
    flipChanged = lastGameState.weplayonpositivehalf() != newGameState.weplayonpositivehalf();
    if(lastRefMessage.has_designated_position()){
        proto::Vector2f designatedPos;
        if(flipField){
            designatedPos.set_x(-lastRefMessage.designated_position().x()/1000.0f);
            designatedPos.set_y(-lastRefMessage.designated_position().y()/1000.0f);
        }else{
            designatedPos.set_x(lastRefMessage.designated_position().x()/1000.0f);
            designatedPos.set_y(lastRefMessage.designated_position().y()/1000.0f);
        }
        newGameState.mutable_designated_position()->CopyFrom(designatedPos);
    }


    //newGameState.set_nextcommand();//TODO command switch
    int gameEventCount = lastRefMessage.game_events_size();
    if(gameEventCount> lastGameEventCount){
        for (int i = lastGameEventCount; i < gameEventCount ; ++ i) {
            newGameState.add_game_events()->CopyFrom(lastRefMessage.game_events(i));
        }
    } else if(lastGameEventCount >gameEventCount){
        //less events so we assume all events are new
        newGameState.mutable_game_events()->CopyFrom(lastRefMessage.game_events());
    }
    lastGameEventCount = gameEventCount;
    if(lastRefMessage.has_current_action_time_remaining()){
        newGameState.set_current_action_time_remaining((long)(lastRefMessage.current_action_time_remaining())*1000);
    }
    proto::Team team = weAreBlue ? proto::Team::BLUE : proto::Team::YELLOW;
    newGameState.set_ourcolor(team);
    return newGameState;
}
//checks whether we are one of the teams playing and if so returns true at sets that to our color
bool RefereeFilter::inferOurColor(const proto::Referee &refereeMessage) {
    //this name should be the same as our name in  https://github.com/RoboCup-SSL/ssl-game-controller/blob/master/src/components/settings/team/TeamName.vue
    //or whatever the game client is at that point
    const std::string RTT_NAME ="RoboTeam Twente";
    if(refereeMessage.blue().name() == RTT_NAME){
        return true; // we are blue
    }
    return refereeMessage.yellow().name() != RTT_NAME;    // We default to being blue
}
bool RefereeFilter::flipHasChanged() const {
    return flipChanged;
}
