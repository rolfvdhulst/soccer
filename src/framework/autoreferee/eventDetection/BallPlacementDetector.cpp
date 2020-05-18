//
// Created by rolf on 17-5-20.
//

#include "eventDetection/BallPlacementDetector.h"
std::vector<proto::GameEvent> BallPlacementDetector::update(const Context &context) {
    const WorldState &world = context.currentWorld();
    if(context.commandChanged && context.referee.command.isBallPlacement()){
        if(world.hasBall()&&context.referee.designatedPosition != std::nullopt){
            startingDistance = (world.getBall()->pos()-*context.referee.designatedPosition).length();
            startingPos = world.getBall()->pos();
        }else{
            startingDistance = std::nullopt;
            startingPos = std::nullopt;
        }
        startingTime = world.getTime();
    }
    auto actionTime = context.referee.currentActionTimeRemaining;
    if(actionTime != std::nullopt && *actionTime<Time(0.0)){
        proto::GameEvent event;
        proto::GameEvent_PlacementFailed * failure = event.mutable_placement_failed();
        if(GameCommand::BALL_PLACEMENT_US == context.referee.command){
            failure->set_by_team(proto::Team::BLUE);
        } else if(GameCommand::BALL_PLACEMENT_THEM == context.referee.command){
            failure->set_by_team(proto::Team::YELLOW);
        } else{
            failure->set_by_team(proto::Team::UNKNOWN);
        }
        auto ball = context.currentWorld().getBall();
        if(ball && context.referee.designatedPosition){
            failure->set_remaining_distance((ball->pos()-*context.referee.designatedPosition).length());
        }
        return {event};
    }
    //If there is no recent world information we don't send anything
    if(context.worldHistory.empty()){
        return {};
    }

    if(!world.hasBall() || context.referee.designatedPosition == std::nullopt){
        return {};
    }
    BallState ball = *world.getBall();
    Vector2 placementPos = *context.referee.designatedPosition;
    //Check if ball is within required 15 centimeter
    double remainingDistance =(ball.pos()-placementPos).length();
    bool ballAtPosition = remainingDistance<TOLERANCE_RADIUS;
    //check if ball is lying still
    bool ballStationary = ball.vel().length() < STATIONARY_BALL_SPEED;


    //check if every robot is atleast minimum distance from the ball

    double minimumBlueDistance = 0.5;
    double minimumYellowDistance = 0.5;
    if(context.referee.nextCommand){
        if( GameCommand::DIRECT_FREE_US == *context.referee.nextCommand){
            minimumBlueDistance = 0.05;
        }else if(GameCommand::DIRECT_FREE_THEM == *context.referee.nextCommand){
            minimumYellowDistance = 0.05;
        }
    }
    bool robotsFarAway = true;
    for(const auto& robot : world.getUs()){
        if(robot.shape().distanceTo(ball.pos())<minimumBlueDistance){
            robotsFarAway = false;
        }
    }
    for(const auto& robot : world.getThem()){
        if(robot.shape().distanceTo(ball.pos())<minimumYellowDistance){
            robotsFarAway = false;
        }
    }

    if(ballAtPosition && ballStationary && robotsFarAway){
        proto::GameEvent event;
        proto::GameEvent_PlacementSucceeded * success = event.mutable_placement_succeeded();
        if(GameCommand::BALL_PLACEMENT_US == context.referee.command){
            success->set_by_team(proto::Team::BLUE);
        } else if(GameCommand::BALL_PLACEMENT_THEM == context.referee.command){
            success->set_by_team(proto::Team::YELLOW);
        } else{
            success->set_by_team(proto::Team::UNKNOWN);
        }
        success->set_precision(remainingDistance);
        if(startingDistance){
            success->set_distance(*startingDistance);
        }
        success->set_time_taken((world.getTime()-startingTime).asSeconds());

        return {event};
    }
    return {};
}
bool BallPlacementDetector::isApplicable(const GameCommand &command) const {
    return command.isBallPlacement();
}
BallPlacementDetector::BallPlacementDetector() : SingleEventDetector() {

}
