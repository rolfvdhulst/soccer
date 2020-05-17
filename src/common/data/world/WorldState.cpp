//
// Created by rolf on 23-2-20.
//

#include "WorldState.h"
const std::vector<RobotState> &WorldState::getUs() const {
    return us;
}
const std::vector<RobotState> &WorldState::getThem() const {
    return them;
}
const std::vector<RobotState> &WorldState::getTeam(WorldTeam team) const {
    switch(team){
    case WorldTeam::US: return getUs();
    case WorldTeam::THEM: return getThem();
    }
}
const std::vector<RobotState> &WorldState::getAllRobots() const {
    return robots;
}
const Time &WorldState::getTime() {
    return time;
}
unsigned int WorldState::ourRobotCount() const {
    return us.size();
}
unsigned int WorldState::theirRobotCount() const {
    return them.size();
}
bool WorldState::weHaveRobots() const {
    return ourRobotCount()>0;
}
bool WorldState::theyHaveRobots() const {
    return theirRobotCount();
}
bool WorldState::hasBall() const {
    return ball.has_value();
}
std::optional<BallState> WorldState::getBall() const {
    return ball;
}
std::optional<RobotState> WorldState::getRobot(const RobotID &id, WorldTeam team) const {
    const std::vector<RobotState> & teamRobots = team == WorldTeam::US ? us : them;
    //TODO: use std::find_if, maybe?
    for (const auto& robot : teamRobots){
        if(robot.id() == id){
            return robot;
        }
    }
    return std::nullopt;
}
std::optional<RobotState> WorldState::getOurRobot(const RobotID &id) const {
    return getRobot(id,WorldTeam::US);
}
std::optional<RobotState> WorldState::getTheirRobot(const RobotID &id) const{
    return getRobot(id,WorldTeam::THEM);
}
bool WorldState::hasRobots() const {
    return weHaveRobots() || theyHaveRobots();
}
WorldState::WorldState(const proto::World &world, bool weAreBlue, const proto::TeamRobotInfo& teamRobotInfo) : time(world.time())
{
  for(const auto& robot : world.blue()){
    us.emplace_back(RobotState(robot,teamRobotInfo.blue()));
  }
  for(const auto& robot : world.yellow()){
    them.emplace_back(RobotState(robot,teamRobotInfo.yellow()));
  }
  if(!weAreBlue){
    std::swap(us,them);
  }
  if(world.has_ball()){
    ball = BallState(world.ball());
  }else{
    ball = std::nullopt;
  }
  robots.insert(robots.begin(),us.begin(),us.end());
  robots.insert(robots.end(),them.begin(),them.end());
}

