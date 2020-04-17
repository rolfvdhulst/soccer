//
// Created by rolf on 29-03-20.
//

#include <protobuf/World.pb.h>
#include "RefereeFilter.h"
#include <geometry/CommandSwitch.h>//TODO: find nice place that avoids circular dependencies

proto::GameState RefereeFilter::update(const proto::Settings &settings,
        const std::vector<proto::Referee> &refereeMessages, const proto::World &world) {
    flipChanged = false;
    proto::GameState newGameState = lastGameState;
    if (refereeMessages.empty()) {
        updateNoMessages(newGameState, settings, world);
    }
    else {
        //Note the below line assumes that the referee messages are sorted ascending by time!
        const proto::Referee &lastRefMessage = refereeMessages.back();
        updateWithMessage(newGameState, settings, lastRefMessage, world);
    }

    lastGameState = newGameState;
    if (lastGameState.has_referee()) {
        lastGameState.mutable_referee()->clear_game_events();
    }
    return newGameState;
}

//checks whether we are one of the teams playing and if so returns true at sets that to our color
bool RefereeFilter::inferOurColor(const proto::Settings &settings, const proto::Referee &refereeMessage) {
    //this name should be the same as our name in  https://github.com/RoboCup-SSL/ssl-game-controller/blob/master/src/components/settings/team/TeamName.vue
    //or whatever the game client is at that point
    const std::string RTT_NAME = "RoboTeam Twente";
    if (refereeMessage.blue().name() == RTT_NAME) {
        return true; // we are blue
    }
    else if (refereeMessage.yellow().name() == RTT_NAME) {
        return false; // We are yellow
    }
    return settings.firstteam().weareblue();    // We default to the interface value if we don't find our name
}
bool RefereeFilter::flipHasChanged() const {
    return flipChanged;
}
proto::TeamRobotInfo RefereeFilter::getTeamRobotInfo() const {
    proto::TeamRobotInfo teamRobotInfo;

    std::string blueString;
    std::string yellowString;
    //check if we even have referee message
    if (lastGameState.has_referee()) {
        if (lastGameState.settings().weareblue()) {
            blueString = lastGameState.referee().us().name();
            yellowString = lastGameState.referee().them().name();
        }
        else {
            blueString = lastGameState.referee().them().name();
            yellowString = lastGameState.referee().us().name();
        }
    }
    teamRobotInfo.mutable_blue()->CopyFrom(getTeamInfo(blueString));
    teamRobotInfo.mutable_yellow()->CopyFrom(getTeamInfo(yellowString));
    return teamRobotInfo;
}
RefereeFilter::RefereeFilter() {
    proto::TeamSettings settings;
    settings.set_weareblue(true);
    settings.set_weplayonpositivehalf(false);
    settings.set_keeperid(0);
    lastGameState.mutable_settings()->CopyFrom(settings);
}
void RefereeFilter::updateNoMessages(proto::GameState &newGameState, const proto::Settings &settings,
        const proto::World &world) {
    //set teamSettings from interface if we don't listen to referee
    if (! settings.listentoreferee()) {
        newGameState.mutable_settings()->CopyFrom(settings.firstteam());
        if (newGameState.settings().weplayonpositivehalf() != lastGameState.settings().weplayonpositivehalf()) {
            flipChanged = true;
        }
    }
    //If we listen to the referee changing settings is locked in interface, so we don't need to handle it here.

    //Check for command switches and adjust if this is indeed the case.
    updateCommandInfo(newGameState, world);
}
void RefereeFilter::updateWithMessage(proto::GameState &newGameState, const proto::Settings &settings,
        const proto::Referee &lastRefMessage, const proto::World &world) {
    proto::TeamSettings teamSettings = makeTeamSettings(settings, lastRefMessage);
    newGameState.mutable_settings()->CopyFrom(teamSettings);
    updateRefereeInfo(newGameState, lastRefMessage);
    updateCommandInfo(newGameState, lastRefMessage, world);
}
proto::TeamSettings RefereeFilter::makeTeamSettings(const proto::Settings &settings,
        const proto::Referee &lastRefMessage) {//First define the color
    proto::TeamSettings teamSettings;
    bool weAreBlue;
    if (settings.listentoreferee()) {
        weAreBlue = inferOurColor(settings, lastRefMessage);
    }
    else {
        weAreBlue = settings.firstteam().weareblue();
    }
    teamSettings.set_weareblue(weAreBlue);
    //Set settings
    bool flipField = settings.firstteam().weplayonpositivehalf(); //default to the interface value if we are not listening to referee
    if (settings.listentoreferee()) {
        flipField = ! weAreBlue;
        if (lastRefMessage.has_blue_team_on_positive_half()) {
            flipField = weAreBlue == lastRefMessage.blue_team_on_positive_half();
        }
    }
    teamSettings.set_weplayonpositivehalf(flipField);
    if (teamSettings.weplayonpositivehalf() != lastGameState.settings().weplayonpositivehalf()) {
        flipChanged = true;
    }

    int keeperID = settings.firstteam().keeperid();
    if (settings.listentoreferee()) {
        keeperID = weAreBlue ? lastRefMessage.blue().goalkeeper() : lastRefMessage.yellow().goalkeeper();
    }
    teamSettings.set_keeperid(keeperID);
    return teamSettings;
}
void RefereeFilter::updateRefereeInfo(proto::GameState &newGameState, const proto::Referee &lastRefMessage) {
    proto::RefereeState* refInfo = newGameState.mutable_referee();
    refInfo->set_timestamp(lastRefMessage.packet_timestamp()*1000);
    refInfo->set_stage(lastRefMessage.stage());
    if (lastRefMessage.has_stage_time_left()) {
        refInfo->set_stage_time_left((long) lastRefMessage.stage_time_left()*1000);
    }
    //Commands are added in other place
    refInfo->set_command_counter(lastRefMessage.command_counter());
    refInfo->set_command_timestamp(lastRefMessage.command_timestamp()*1000);

    if (newGameState.settings().weareblue()) {
        refInfo->mutable_us()->CopyFrom(lastRefMessage.blue());
        refInfo->mutable_them()->CopyFrom(lastRefMessage.yellow());
    }
    else {
        refInfo->mutable_us()->CopyFrom(lastRefMessage.yellow());
        refInfo->mutable_them()->CopyFrom(lastRefMessage.blue());
    }

    if (lastRefMessage.has_designated_position()) {
        proto::Vector2f designatedPos;
        if (newGameState.settings().weplayonpositivehalf()) {
            designatedPos.set_x(- lastRefMessage.designated_position().x()/1000.0f);
            designatedPos.set_y(- lastRefMessage.designated_position().y()/1000.0f);
        }
        else {
            designatedPos.set_x(lastRefMessage.designated_position().x()/1000.0f);
            designatedPos.set_y(lastRefMessage.designated_position().y()/1000.0f);
        }
        refInfo->mutable_designated_position()->CopyFrom(designatedPos);
    }

    int gameEventCount = lastRefMessage.game_events_size();
    if (gameEventCount > lastGameEventCount) {
        for (int i = lastGameEventCount; i < gameEventCount; ++ i) {
            refInfo->add_game_events()->CopyFrom(lastRefMessage.game_events(i));
        }
    }
    else if (lastGameEventCount > gameEventCount) {
        //less events so we assume all events are new
        refInfo->mutable_game_events()->CopyFrom(lastRefMessage.game_events());
    }
    lastGameEventCount = gameEventCount;
    if (lastRefMessage.has_current_action_time_remaining()) {
        refInfo->set_current_action_time_remaining((long) (lastRefMessage.current_action_time_remaining())*1000);
    }
}
//Requires the switching logic to be called BEFORE it
void RefereeFilter::updateBallMoved(const proto::World &world) {
    //Check if ball moved in switch. We only set this if it hasn't moved 0.05 yet and if we are in the command switch.
    if (isInCommandSwitch && ! ballMovedInSwitch && ballPosStartSwitch && world.has_ball()
            && (*ballPosStartSwitch - Vector2(world.ball().pos())).length() > 0.05) {
        ballMovedInSwitch = true;
    }
}
void RefereeFilter::updateFirstBallPos(const proto::World &world) {
    //Record the first position we see the ball in the command switch.
    if (isInCommandSwitch && ! ballPosStartSwitch && world.has_ball()) {
        ballPosStartSwitch = Vector2(world.ball().pos());
    }
}
void RefereeFilter::updateSwitchInfo(const std::optional<proto::RefereeState_Command> &switchCommand) {
    if (isInCommandSwitch) {
        if (! switchCommand) {
            isInCommandSwitch = false;
            ballMovedInSwitch = false;
        }
    }
    else {
        ballPosStartSwitch = std::nullopt; //we explicitly set this to null so that we only record the ballPos at the start of the switch
        if (switchCommand) {
            isInCommandSwitch = true;
            ballMovedInSwitch = false;
        }
    }
}
void RefereeFilter::updateCommandInfo(proto::GameState &newGameState, const proto::Referee &lastRefMessage,
        const proto::World &world) {
    //we assume we are blue. If not, we invert the command all the way at the end.
    //proto::RefereeState_Command outCommand = defaultMap(lastRefMessage.command());

    std::optional<proto::RefereeState_Command> switchCommand = getCommand(lastCommand, lastRefMessage.command());

    updateSwitchInfo(switchCommand);
    updateFirstBallPos(world);
    updateBallMoved(world);
    if (lastCommand != lastRefMessage.command() && ! isInCommandSwitch) {
        lastCommand = lastRefMessage.command();
    }
    proto::RefereeState_Command outCommand = defaultMap(lastRefMessage.command());
    if (switchCommand) {
        outCommand = *switchCommand;
        if (ballMovedInSwitch) {
            outCommand = proto::RefereeState_Command_RUNNING;
        }
    }
    //set all relevant details in the new game state
    if (! newGameState.settings().weareblue()) {
        outCommand = invertTeams(outCommand);
    }
    newGameState.mutable_referee()->set_command(outCommand);
    if (lastRefMessage.has_next_command()) {
        if (newGameState.settings().weareblue()) {
            newGameState.mutable_referee()->set_nextcommand(defaultMap(lastRefMessage.next_command()));
        }
        else {
            newGameState.mutable_referee()->set_nextcommand(invertTeams(defaultMap(lastRefMessage.next_command())));
        }
    }
}
void RefereeFilter::updateCommandInfo(proto::GameState &newGameState, const proto::World &world) {
    updateFirstBallPos(world);
    updateBallMoved(world);
    if (isInCommandSwitch && ballMovedInSwitch && newGameState.has_referee()) {
        newGameState.mutable_referee()->set_command(proto::RefereeState_Command_RUNNING);
    }
}

