//
// Created by rolf on 17-07-20.
//

#include "eventDetection/BotCollisionDetector.h"

#include <utility>

BotCollisionDetector::BotCollisionDetector() : SingleEventDetector() {

}

//Is applicable in almost all game states so this is easier
bool BotCollisionDetector::isApplicable(const GameCommand &command) const {
    return GameCommand::HALT != command && !command.isTimeout();
}

std::vector<proto::GameEvent> BotCollisionDetector::update(const Context &context) {
    const WorldState &world = context.currentWorld();
    //If there is no recent world information we don't send anything
    if (context.worldHistory.empty()) {
        return {};
    }
    std::vector<Collision> collisions = detectCollisions(world);
    std::vector<Collision> newCollisions = getNewCollisions(world.getTime(),collisions);

    std::vector<proto::GameEvent> events;
    for(const auto& collision : newCollisions){
        events.push_back(convertToProtoEvent(collision));
    }
    return events;
}

std::vector<BotCollisionDetector::Collision> BotCollisionDetector::detectCollisions(const WorldState &world) {
    std::vector<Collision> collisions;
    for (const auto &blueBot : world.getUs()) {
        for (const auto &yellowBot : world.getThem()) {
            std::optional<Collision> collision = getCollision(world.getTime(), blueBot, yellowBot);
            if (collision) {
                collisions.push_back(*collision);
            }
        }
    }
    return collisions;
}

std::optional<BotCollisionDetector::Collision>
BotCollisionDetector::getCollision(const Time &collisionTime, const RobotState &blueBot,
                                   const RobotState &yellowBot) const {
    //TODO: can do linear extrapolation if robots are a certain distance from each other and assume they will collide and assume a breaking decelleration
    double distanceBetweenBots = blueBot.pos().dist(yellowBot.pos());
    if (distanceBetweenBots > blueBot.radius() + yellowBot.radius()) {
        return std::nullopt;
    }
    Vector2 collisionPos = (blueBot.pos() + yellowBot.pos()) * 0.5; //take the center of the line between the two robots

    bool blueIsFastest = blueBot.vel().length() >= yellowBot.vel().length();
    const RobotState &offender = blueIsFastest ? blueBot : yellowBot;
    const RobotState &victim = blueIsFastest ? yellowBot : blueBot;
    Vector2 velDif = offender.vel() - victim.vel();

    Vector2 collisionPosWithVel = collisionPos + velDif;
    Vector2 projectedVelPos = Line(blueBot.pos(), yellowBot.pos()).project(collisionPosWithVel);
    double projectedVelLength = projectedVelPos.dist(collisionPos);

    double absoluteSpeedDiff = fabs(blueBot.vel().length()-yellowBot.vel().length());
    if (projectedVelLength > COLLISION_SPEED) {
        Collision collision(collisionTime, blueBot, yellowBot, blueIsFastest, collisionPos, projectedVelLength,absoluteSpeedDiff);
        return collision;
    }
    return {};

}

std::vector<BotCollisionDetector::Collision>
BotCollisionDetector::getNewCollisions(const Time &currentTime, const std::vector<Collision> &detectedCollisions) {
    //Clean up old collisions, removing them if they are older than 2.0 seconds
    auto iterator = oldCollisions.begin();
    while (iterator != oldCollisions.end()) {
        if (currentTime - iterator->collisionTime > Time(2.0)) {
            iterator = oldCollisions.erase(iterator);
        } else {
            ++iterator;
        }
    }
    //Check for each detected collision if it's not been sent in the past 2.0 seconds already
    std::vector<Collision> newCollisions;
    for (const auto &detectedCollision: detectedCollisions) {
        bool collisionIsNew = true;
        //inefficient search data structure but it doesn't really matter since there's barely any collisions anyway
        for (const auto &oldCollision : oldCollisions) {
            if (oldCollision.blueRobot.id() == detectedCollision.blueRobot.id() &&
                oldCollision.yellowRobot.id() == detectedCollision.yellowRobot.id()
                    ) {
                collisionIsNew = false;
                break;
            }
        }
        if (collisionIsNew) {
            newCollisions.push_back(detectedCollision);
            oldCollisions.push_back(detectedCollision);
        }
    }
    return newCollisions;
}

proto::GameEvent BotCollisionDetector::convertToProtoEvent(const BotCollisionDetector::Collision &collision) {
    proto::GameEvent event;
    double crashAngle = collision.blueRobot.vel().toAngle().angleDiff(collision.yellowRobot.vel().toAngle());
    if (collision.offender == BOTH) {
        event.set_type(proto::BOT_CRASH_DRAWN);
        proto::GameEvent_BotCrashDrawn *eventData = event.mutable_bot_crash_drawn();
        eventData->set_bot_yellow(collision.yellowRobot.id().getID());
        eventData->set_bot_blue(collision.blueRobot.id().getID());
        eventData->mutable_location()->set_x(collision.collisionLocation.x());
        eventData->mutable_location()->set_y(collision.collisionLocation.y());
        eventData->set_speed_diff(collision.absoluteVelocityDifference);
        eventData->set_crash_speed(collision.projectedVelocity);
        eventData->set_crash_angle(crashAngle);
        return event;
    }
    bool blueIsOffender = collision.offender == BLUE;
    proto::GameEvent_BotCrashUnique *eventData = event.mutable_bot_crash_unique();
    event.set_type(proto::BOT_CRASH_UNIQUE);
    eventData->set_by_team(blueIsOffender ? proto::BLUE : proto::YELLOW);
    eventData->set_victim(blueIsOffender ? collision.yellowRobot.id().getID() : collision.blueRobot.id().getID());
    eventData->set_violator(blueIsOffender ? collision.blueRobot.id().getID() : collision.yellowRobot.id().getID());
    eventData->mutable_location()->set_x(collision.collisionLocation.x());
    eventData->mutable_location()->set_y(collision.collisionLocation.y());
    eventData->set_speed_diff(collision.absoluteVelocityDifference);
    eventData->set_crash_speed(collision.projectedVelocity);
    eventData->set_crash_angle(crashAngle);
    return event;
}

BotCollisionDetector::Collision::Collision(const Time &time, RobotState blue, RobotState yellow,
                                           bool blueFaster, const Vector2 &collisionLocation, double projectedVelocity,
                                           double absoluteVelocityDifference)
        :
        collisionTime(time),
        blueRobot(std::move(blue)),
        yellowRobot(std::move(yellow)),
        collisionLocation(collisionLocation),
        projectedVelocity(projectedVelocity),
        absoluteVelocityDifference(absoluteVelocityDifference) {
    if (absoluteVelocityDifference < 0.3) { //The limit for counting 2 collisions as equal
        offender = BOTH;
    } else {
        offender = blueFaster ? BLUE : YELLOW;
    }
}
