//
// Created by rolf on 06-07-20.
//

#include "eventDetection/AttackerToDefenseAreaDistanceDetector.h"
bool AttackerToDefenseAreaDistanceDetector::isApplicable(const GameCommand &command) const {
    return command.isFreeKick() || GameCommand::STOP == command;
}

AttackerToDefenseAreaDistanceDetector::AttackerToDefenseAreaDistanceDetector() : SingleEventDetector(){

}

std::vector<proto::GameEvent> AttackerToDefenseAreaDistanceDetector::update(const Context &context) {
    if(context.worldHistory.empty()){
        return {};
    }

    const WorldState& world = context.currentWorld();
    const Time& currentTime = world.getTime();
    //if a new state occurs reset the grace period and offenders
    if (context.commandChanged) {
        gamestateStartTime = currentTime;
        yellowOffenders.clear();
        blueOffenders.clear();
    }
    //In the first 2 seconds after the command we don't send events
    if(currentTime-gamestateStartTime< GRACE_PERIOD){
        return {};
    }
    removeOldOffenders(true,currentTime);
    removeOldOffenders(false,currentTime);
    std::vector<proto::GameEvent> blueEvents = checkRobots(true,currentTime,context);
    std::vector<proto::GameEvent> yellowEvents = checkRobots(false,currentTime,context);

    blueEvents.insert(blueEvents.end(),yellowEvents.begin(),yellowEvents.end()); //merge all events into one vector
    return blueEvents;
}

void AttackerToDefenseAreaDistanceDetector::removeOldOffenders(bool blue, const Time &currentTime) {
    std::map<RobotID, Time> & offenders = blue ? blueOffenders : yellowOffenders;
    auto iterator = offenders.begin();
    while(iterator!=offenders.end()){
        if(iterator->second + OFFENSE_RESEND_TIME < currentTime){
            iterator = offenders.erase(iterator);
        }else{
            ++iterator;
        }
    }
}
bool AttackerToDefenseAreaDistanceDetector::hasRecentlyOffended(bool blue, RobotID id) const {
    const std::map<RobotID, Time>& offenders = blue ? blueOffenders : yellowOffenders;
    return offenders.find(id) != offenders.end();
}

std::vector<proto::GameEvent>
AttackerToDefenseAreaDistanceDetector::checkRobots(bool blue, const Time &currentTime, const Context &context) {
    std::vector<proto::GameEvent> events;
    const std::vector<RobotState>& robots = blue ? context.currentWorld().getUs() : context.currentWorld().getThem();
    std::map<RobotID,Time> & offenders = blue ? blueOffenders : yellowOffenders;
    const Rectangle& defenceArea = context.geometry.getDefenceArea(!blue); //Get Defence Area of other team

    for (const auto& robot : robots){
        double distanceFromDefenceArea = defenceArea.distanceTo(robot.pos());
        //We also check if the bot is not in the defenseArea
        bool botTooClose = distanceFromDefenceArea < DEFENSE_AREA_DISTANCE + robot.radius() - ERROR_MARGIN || defenceArea.contains(robot.pos());
        if(botTooClose && !hasRecentlyOffended(blue,robot.id())){
            proto::GameEvent event;
            event.set_type(proto::ATTACKER_TOO_CLOSE_TO_DEFENSE_AREA);
            proto::GameEvent_AttackerTooCloseToDefenseArea * violation = event.mutable_attacker_too_close_to_defense_area();
            violation->set_by_team(blue ? proto::Team::BLUE : proto::Team::YELLOW);
            violation->set_by_bot(robot.id().getID());
            violation->mutable_location()->set_x(robot.pos().x());
            violation->mutable_location()->set_y(robot.pos().y());
            violation->set_distance(distanceFromDefenceArea);
            events.push_back(event);
            //Add robot to list of known offenders
            offenders[robot.id()] = currentTime;
        }
    }
    return events;
}
