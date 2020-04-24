//
// Created by rolf on 29-03-20.
//

#include <protobuf/messages_robocup_ssl_geometry.pb.h>
#include "Flip.h"
#include <math/geometry/Angle.h>
#include "CommandSwitch.h"
void flipRobot(proto::WorldRobot * robot){
    robot->mutable_pos()->set_x(robot->pos().x() * -1);
    robot->mutable_pos()->set_y(robot->pos().y() * -1);
    robot->mutable_vel()->set_x(robot->vel().x() * -1);
    robot->mutable_vel()->set_y(robot->vel().y() * -1);
    robot->set_w(robot->w() * -1);
    robot->set_angle(static_cast<float>(Angle(robot->angle() + M_PI).getAngle()));
}
void flipBall(proto::WorldBall * ball){
    ball->mutable_pos()->set_x(ball->pos().x() * -1);
    ball->mutable_pos()->set_y(ball->pos().y() * -1);
    ball->mutable_vel()->set_x(ball->vel().x() * -1);
    ball->mutable_vel()->set_y(ball->vel().y() * -1);
}
void flip(proto::World &world){
    if(world.has_ball()){
        flipBall(world.mutable_ball());
    }
    // rotate all blue robots
    for (int i = 0; i < world.mutable_blue()->size(); i++) {
        flipRobot(world.mutable_blue(i));
    }

    // rotate all yellow robots
    for (int i = 0; i < world.mutable_yellow()->size(); i++) {
        flipRobot(world.mutable_yellow(i));
    }
}
void flipCamera(proto::SSL_GeometryCameraCalibration * camera){
    camera->set_derived_camera_world_tx(camera->derived_camera_world_tx()*-1);
    camera->set_derived_camera_world_ty(camera->derived_camera_world_tx()*-1);
    //TODO: Note this does not yet rotate any of the intrinsic camera parameters used for projection computations!
}
void flipArc(proto::SSL_FieldCicularArc * arc){
    arc->mutable_center()->set_x(arc->center().x() * -1);
    arc->mutable_center()->set_y(arc->center().y() * -1);
    arc->set_a1(arc->a1() * -1);
    arc->set_a2(arc->a2() * -1);
}
namespace{
std::map<std::string,std::string> nameTransfers= {
        {"TopTouchLine","BottomTouchLine"},
        {"BottomTouchLine","TopTouchLine"},
        {"LeftGoalLine","RightGoalLine"},
        {"RightGoalLine","LeftGoalLine"},
        {"LeftPenaltyStretch","RightPenaltyStretch"},
        {"RightPenaltyStretch","LeftPenaltyStretch"},
        {"LeftFieldLeftPenaltyStretch","RightFieldRightPenaltyStretch"},
        {"RightFieldRightPenaltyStretch","LeftFieldLeftPenaltyStretch"},
        {"LeftFieldRightPenaltyStretch","RightFieldLeftPenaltyStretch"},
        {"RightFieldLeftPenaltyStretch","LeftFieldRightPenaltyStretch"}
};
}
void flipLine(proto::SSL_FieldLineSegment * line){
    auto it =nameTransfers.find(line->name());
    if(it != nameTransfers.end()){
        line->set_name(it->second);
    }
    line->mutable_p1()->set_x(line->p1().x() * -1);
    line->mutable_p1()->set_y(line->p1().y() * -1);
    line->mutable_p2()->set_x(line->p2().x() * -1);
    line->mutable_p2()->set_y(line->p2().y() * -1);
}
void flip(proto::SSL_GeometryData &geometry){
    if(geometry.has_field()){
        flip(geometry.mutable_field());
    }
    for (int i = 0; i < geometry.calib_size(); ++ i) {
        flipCamera(geometry.mutable_calib(i));
    }
}
void flip(proto::SSL_GeometryFieldSize * field){
    for (int i = 0; i < field->mutable_field_lines()->size(); i++) {
        flipLine(field->mutable_field_lines(i));
    }

    // rotate all field arcs
    for (int i = 0; i < field->mutable_field_arcs()->size(); i++) {
        flipArc(field->mutable_field_arcs(i));
    }
}
void flip(proto::SSL_DetectionFrame * frame){
    for (int i = 0; i < frame->balls_size(); ++ i) {
        flip(frame->mutable_balls(i));
    }
    for (int i = 0; i < frame->robots_blue_size(); ++i){
        flip(frame->mutable_robots_blue(i));
    }
    for (int i = 0; i < frame->robots_yellow_size(); ++i){
        flip(frame->mutable_robots_yellow(i));
    }
}
void flip(proto::SSL_DetectionRobot * robot){
    robot->set_x(robot->x()*-1);
    robot->set_y(robot->y()*-1);
    robot->set_orientation(robot->orientation()+M_PIf32);
}
void flip(proto::SSL_DetectionBall * ball){
    ball->set_x(ball->x()*-1);
    ball->set_y(ball->y()*-1);
}
void flipAndSwap(double &a, double &b){
    a *= -1;
    b *= -1;
    std::swap(a, b);
}
void flipAndSwap(Vector2 &a, Vector2 &b){
a *= -1;
b *= -1;
std::swap(a, b);
}
void flipAndSwap(LineSegment &a, LineSegment &b){
flipAndSwap(a.start, b.start);
flipAndSwap(a.end, b.end);
}
void flipAndSwap(Rectangle &a, Rectangle &b){
flipAndSwap(a.min, b.max);
flipAndSwap(a.max, b.min);
}
void flip(proto::GameState& gameState){
    gameState.mutable_settings()->set_weplayonpositivehalf(!gameState.settings().weplayonpositivehalf());
    gameState.mutable_settings()->set_weareblue(!gameState.settings().weareblue());
    if(gameState.has_referee()){
        gameState.mutable_referee()->set_command(invertTeams(gameState.referee().command()));
        if(gameState.referee().has_nextcommand()){
            gameState.mutable_referee()->set_nextcommand(invertTeams(gameState.referee().nextcommand()));
        }
        //swap but have to do this since std:swap does not work on protobuf
        proto::Referee_TeamInfo us = gameState.referee().us();
        gameState.mutable_referee()->mutable_us()->CopyFrom(gameState.referee().them());
        gameState.mutable_referee()->mutable_them()->CopyFrom(us);
        if(gameState.referee().has_designated_position()){
            gameState.mutable_referee()->mutable_designated_position()->set_x(gameState.referee().designated_position().x()*-1);
            gameState.mutable_referee()->mutable_designated_position()->set_y(gameState.referee().designated_position().y()*-1);
        }
    }
    //We ignore game events.
    // Technically they should also be flipped as they contain positions and velocities (sometimes) but this quickly becomes a HUGE switch statement
}