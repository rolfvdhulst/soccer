//
// Created by rolf on 29-03-20.
//

#include <protobuf/World.pb.h>
#include "RefereeFilter.h"
#include "CommandSwitch.h"

proto::GameState RefereeFilter::update(const std::vector<proto::Referee>& refereeMessages, const proto::World& world) {
    flipChanged = false;
    if(refereeMessages.empty()){
        return lastGameState;
    }
    //Note the below line assumes that the referee messages are sorted ascending by time!
    const proto::Referee lastRefMessage=refereeMessages.back();
    bool weAreBlue=inferOurColor(lastRefMessage);
    proto::GameState newGameState = createGameState(lastRefMessage,weAreBlue,world);
    lastGameState = newGameState;
    lastGameState.clear_game_events();
    return newGameState;
}
proto::GameState RefereeFilter::createGameState(const proto::Referee &lastRefMessage,bool weAreBlue,const proto::World& world) {
    proto::GameState newGameState;
    newGameState.set_timestamp(lastRefMessage.packet_timestamp()*1000);
    newGameState.set_stage(lastRefMessage.stage());
    if(lastRefMessage.has_stage_time_left()){
        newGameState.set_stage_time_left((long)lastRefMessage.stage_time_left()*1000);
    }
    //actual command is added in addCommands() function together with predicted command
    addCommands(newGameState,lastRefMessage,weAreBlue,world);
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
void RefereeFilter::addCommands(proto::GameState &gameState, const proto::Referee &lastRefMessage, bool weAreBlue,
        const proto::World &world) {
    //we assume we are blue. If not, we invert the command all the way at the end.
    //I am sorry for the code. There must be a pretty way. I just haven't found it.
    proto::GameState_Command outCommand = proto::GameState_Command_HALT;
    if(isInCommandSwitch && !ballMovedInSwitch && ballPosStartSwitch && world.has_ball()
    && (*ballPosStartSwitch-Vector2(world.ball().pos())).length()>0.05){
        ballMovedInSwitch = true;
    }
    std::optional<proto::GameState_Command> switchCommand = getCommand(lastCommand,lastRefMessage.command());
    if(isInCommandSwitch){
        if(switchCommand){
            if(ballMovedInSwitch){
                outCommand = proto::GameState_Command_RUNNING;
            }else{
                outCommand =*switchCommand;
            }
        }else{
            isInCommandSwitch = false;
            ballMovedInSwitch = false;
        }
    }else{
        ballPosStartSwitch = std::nullopt;
        if (switchCommand){
            isInCommandSwitch = true;
            ballMovedInSwitch = false;
            outCommand = *switchCommand;
        }
    }
    if(!isInCommandSwitch){
        outCommand = defaultMap(lastRefMessage.command());
    }
    if(isInCommandSwitch && !ballPosStartSwitch && world.has_ball()){
        ballPosStartSwitch = Vector2(world.ball().pos());
    }
    if(lastCommand != lastRefMessage.command() && !isInCommandSwitch){
        lastCommand = lastRefMessage.command();
    }
    if(!weAreBlue){
        outCommand=invertTeams(outCommand);
    }
    gameState.set_command(outCommand);
    if(lastRefMessage.has_next_command()){
        if(weAreBlue){
            gameState.set_nextcommand(defaultMap(lastRefMessage.next_command()));
        }
        else{
            gameState.set_nextcommand(invertTeams(defaultMap(lastRefMessage.next_command())));
        }
    }
}
