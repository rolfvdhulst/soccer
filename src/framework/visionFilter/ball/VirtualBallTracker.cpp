//
// Created by rolf on 15-09-20.
//

#include "ball/VirtualBallTracker.h"
#include <math/geometry/LineSegment.h>
#include <math/geometry/RobotShape.h>

void VirtualBallTracker::clearVirtualBalls() {
    virtualBalls.clear();
}

void VirtualBallTracker::updateVirtualBalls(const std::vector<FilteredRobot> &robots,
                                            const Camera &camera,
                                            const RobotParameters &parameters,
                                            const DetectionFrame &detectionFrame) {
    for (auto &virtualBall : virtualBalls) {
        //TODO: make position based of ball, robotID and team can be duplicate in exceptional cases
        bool virtualBallIsVisible = false;
        bool robotOnFrame = false;
        Eigen::Vector3d dribbledBallPos(0, 0, 0);
        for (const auto &robot : robots) {
            if (robot.id == virtualBall.robotID && robot.isBlue == virtualBall.isBlue) {
                dribbledBallPos = getDribbledBallPos(robot, parameters);
                virtualBallIsVisible = ballShouldBeVisible(dribbledBallPos, robots, camera,parameters);
                if (virtualBallIsVisible) {
                    ++virtualBall.ticksShouldHaveBeenSeen;
                }
                robotOnFrame = true;
                break;
            }
        }
        if (robotOnFrame) {
            for (const auto &detectionBall : detectionFrame.balls) {
                if ((detectionBall.position - Eigen::Vector2d(dribbledBallPos.x(),dribbledBallPos.y())).norm() < 0.05) {
                    ++virtualBall.ticksActuallySeen;
                    if (virtualBallIsVisible) {
                        --virtualBall.ticksActuallySeen;
                    }
                    break;
                }
            }
        }
    }
    //Delete balls that clearly should have been seen
    constexpr int maxTicks = 20;
    auto it = virtualBalls.begin();
    while (it != virtualBalls.end()) {
        if (it->ticksShouldHaveBeenSeen > maxTicks) {
            it = virtualBalls.erase(it);
        } else {
            ++it;
        }
    }

}

Eigen::Vector3d VirtualBallTracker::getDribbledBallPos(const FilteredRobot &robot,
                                                       const RobotParameters &parameters) const {
    const double ballRadius = 0.021333;//TODO: make constant somewhere
    Eigen::Vector2d position = robot.pos + Eigen::Rotation2D<double>(robot.angle) *
                                           Eigen::Vector2d(parameters.centerToFront + ballRadius,
                                                           0); //TODO: check if rotation happens correctly
    return Eigen::Vector3d(position.x(), position.y(), ballRadius);
}

bool VirtualBallTracker::ballShouldBeVisible(const Eigen::Vector3d &ballPos, const std::vector<FilteredRobot> &robots,
                                             const Camera &camera, const RobotParameters& parameters) const {
    if(!camera.isPositionVisible(ballPos * 1000.0,0.1)){
        return false;
    }
    //Check if robots intersect line
    Eigen::Vector3d cameraPos = camera.worldPos();
    LineSegment floorLine(Vector2(cameraPos.x(),cameraPos.y()),Vector2(ballPos.x(),ballPos.y()));
    for(const auto& robot : robots){
        RobotShape robotShape(Vector2(robot.pos),parameters.centerToFront,parameters.radius,Angle(robot.angle));

        std::vector<Vector2> intersects = robotShape.intersects(floorLine);
        for(const auto& intersect : intersects){
            double t = (intersect - floorLine.start()).length() / floorLine.length();
            double height = cameraPos.z() + t*(ballPos.z()-cameraPos.z());
            if(height<=parameters.height){
                std::cout<<"robot "<<robot.id<<" is blue: "<<robot.isBlue<<" in the way!"<<std::endl;
                return false;
            }
        }
    }
    return true;
}

std::vector<VirtualBallTracker::VirtualBall> VirtualBallTracker::getViableVirtualBalls(const std::vector<FilteredRobot>& goodRobots, const RobotParameters& parameters) const {
    //sort balls based on criterion
    auto sortFunction = [](const RobotDribblingBall &first, const RobotDribblingBall &second) {
        return first.ticksActuallySeen * 2 - first.ticksShouldHaveBeenSeen >
               second.ticksActuallySeen * 2 - second.ticksShouldHaveBeenSeen;
    };
    std::vector<RobotDribblingBall> dribblingBalls= virtualBalls;
    std::sort(dribblingBalls.begin(),dribblingBalls.end(),sortFunction);

    std::vector<VirtualBall> balls;
    for(const auto& ball : dribblingBalls){
        for(const auto& robot : goodRobots){
            if(robot.id == ball.robotID && robot.isBlue == ball.isBlue){
                VirtualBall virtualBall;
                virtualBall.ball = ball;
                auto vec = getDribbledBallPos(robot,parameters);
                virtualBall.x = vec.x();
                virtualBall.y = vec.y();
                balls.push_back(virtualBall);
            }
        }
    }

    //only pass back the 3 most likely balls
    constexpr int maxSize = 3;
    if(balls.size()>maxSize){
        balls.resize(maxSize);
    }
    return balls;
}

void VirtualBallTracker::generateVirtualBalls(const std::vector<FilteredRobot> &robots,
                                              const BallObservation& lastBallSeenPos) {
    clearVirtualBalls();
    double radius = 0.5;
    for(const auto& robot : robots){
        if((robot.pos - lastBallSeenPos.position).norm()<radius){
            virtualBalls.emplace_back(robot.id,robot.isBlue);
        }
    }
    if(!virtualBalls.empty()){
        std::cout<<"virtual Balls: "<<virtualBalls.size()<<std::endl;
        return;
    }
    radius = 1.5;
    for(const auto& robot : robots){
        if((robot.pos - lastBallSeenPos.position).norm()<radius){
            virtualBalls.emplace_back(robot.id,robot.isBlue);
        }
    }
    std::cout<<"virtual Balls: "<<virtualBalls.size()<<std::endl;
}

proto::WorldVirtualBall VirtualBallTracker::VirtualBall::asProto() const {
    proto::WorldVirtualBall protoBall;
    protoBall.mutable_pos()->set_x(x);
    protoBall.mutable_pos()->set_y(y);
    protoBall.set_teamisblue(ball.isBlue);
    protoBall.set_dribblingrobotid(ball.robotID);
    protoBall.set_framesseenfor(ball.ticksActuallySeen);
    protoBall.set_missedframes(ball.ticksShouldHaveBeenSeen);
    return protoBall;
}
