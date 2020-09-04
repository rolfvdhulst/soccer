//
// Created by rolf on 11-08-20.
//

#include "CollisionChecker.h"
#include <math/geometry/LineSegment.h>
#include <math/geometry/Line.h>
#include <math/general/QuadraticEquation.h>
#include <math/geometry/Polygon.h>

namespace CollisionChecker{
    std::optional<CollisionPreliminaryResult> getFieldOutsideWallCollision(const BallTrajectorySegment& ballSegment, const GeometryData& geometryData){
        //TODO: add check if line is completely within field without margin. If so, simply return nullopt.
        LineSegment ballPath = ballSegment.path(); //assumes the ball moves in a straight line
        const double ballRadius = 0.021333; //TODO: fix this being local
        //Get outside wall lines where the ball would collide
        Rectangle outerWall = geometryData.field.getFieldCorrected(geometryData.field.getBoundaryWidth()-ballRadius);
        if(outerWall.contains(ballSegment.startPos)&&outerWall.contains(ballSegment.endPos))
        if(!outerWall.contains(ballSegment.startPos)){ //check if the ball is inside the collision margin.
            //If we don't do this, sometimes the ball after a collision will be seen as outside of the field and collide with the outside again
            // because it's velocity points into the field
            //TODO: return second closest collision
            return std::nullopt;
        }

        //TODO: just make a vector with collisions
        std::optional<double> bestFraction = std::nullopt;
        std::optional<Vector2> normalDir = std::nullopt;
        for(const auto & wallLine: outerWall.lines()){
            std::optional<double> collisionFraction = ballPath.intersectsParametrize(wallLine);
            if(collisionFraction ){
                if(!bestFraction || *collisionFraction<*bestFraction){
                    bestFraction = collisionFraction;
                    //If the collision point is closer or does not exist, we register it as the best point
                    Vector2 dir = wallLine.direction();
                    normalDir = Vector2(dir.y(),-dir.x());
                }
            }
        }
        if(!bestFraction){
            return std::nullopt;
        }
        CollisionPreliminaryResult result;
        result.type = CollisionType::OUTER_WALL;
        result.normalDir = *normalDir;
        result.distanceFraction = *bestFraction;
        return result;
    }

    std::optional<CollisionPreliminaryResult> getFieldGoalWallCollision(const BallTrajectorySegment& ballSegment, const GeometryData& geometryData){
        LineSegment ballPath = ballSegment.path();
        const double ballRadius = 0.021333; //TODO: fix this being local
        std::optional<CollisionPreliminaryResult> bestCollision = std::nullopt;

        //Check both goals
        //TODO: check if goal coordinates correspond to field properly and fill in
        for(double dir : {-1.0, 1.0}){
            //Define goal with a margin
            Vector2 backLeft;
            Vector2 frontRightPostRight;
            Rectangle rect(backLeft,frontRightPostRight);
            if(!rect.containsOrOverlaps(ballPath)){//pruning: if the line does not overlap this (which is a much faster check), there's 100% no collisions
                continue;
            };

            Vector2 backRight;
            Vector2 frontRightPostLeft;
            Vector2 insideRight;
            Vector2 insideLeft;
            Vector2 frontLeftPostRight;
            Vector2 frontLeftPostLeft;

            Polygon<8> goalWall(frontRightPostRight,frontRightPostLeft,insideRight,insideLeft,frontLeftPostRight,
                                frontLeftPostLeft,backLeft,backRight);
            if(goalWall.contains(ballSegment.startPos)){
                continue; //We don't count collisions if the ball is within the goal's walls
                //TODO: count second best collisions in this case
            }
            auto boundary = goalWall.getBoundary();
            int i = 0; // we count to check which goal segment we're checking. Changing the polygon WILL change this!
            for(const auto& line : boundary){
                std::optional<double> collisionFraction = ballPath.intersectsParametrize(line);
                if(collisionFraction){
                    if(!bestCollision || *collisionFraction < bestCollision->distanceFraction){
                        CollisionPreliminaryResult result;
                        result.distanceFraction = *collisionFraction;
                        Vector2 dir = line.direction();
                        result.normalDir =  Vector2(dir.y(),-dir.x());
                        result.type = (i>0 && i <4) ? CollisionType::GOAL_INSIDE : CollisionType::GOAL_OUTSIDE;
                        //If the collision point is closer or does not exist, we register it as the best point
                    }
                }
                ++i;//TODO: check which lines are collided with in a more pretty way?
            }
        }
        return bestCollision;
    }

    Collision fieldCollideAndReflect(const BallTrajectorySegment& segment, const CollisionPreliminaryResult& preliminary){
        Vector2 collisionPos = segment.path().getPos(preliminary.distanceFraction);
        //Solve the time. Since we choose the fraction between 0 and 1, we are guaranteed a solution.
        assert(preliminary.distanceFraction<=1 && preliminary.distanceFraction>=0);
        double a = 0.5 * segment.acc;
        double b = segment.startVel.length();
        double c = - preliminary.distanceFraction*(b*segment.dt+0.5*segment.acc*segment.dt*segment.dt);
        auto times = solveQuadratic(a,b,c);
        double time;
        if(times){
            time = (times->first<=segment.dt) ? times->first : times->second; //we need the smaller root, which is guaranteed <dt through the collision calculation
        } else{
            time = preliminary.distanceFraction*segment.dt; //use a constant velocity model in case quadratic equation somehow fails. This usually is not far off
        }

        Vector2 collisionVel = segment.getVelocity(time);

        //mirror velocity in collision normal
        //one can also use angles, though that's probably slower and less numerically stable
        Line normalThroughCollision(collisionPos,collisionPos+preliminary.normalDir);
        Vector2 beforeLinePos = (collisionPos-collisionVel);
        Vector2 projection =  normalThroughCollision.project(beforeLinePos);
        Vector2 afterLinePos = projection*2.0 - beforeLinePos;// A +(B-A)*2 = B*2 -A
        double restitution = getRestitution(preliminary.type);
        Vector2 outVel = (afterLinePos-collisionPos).stretchToLength( restitution*collisionVel.length());

        //TODO: make this constructor
        Collision collision;
        collision.collisionTime = segment.startTime + Time(time);
        collision.dt = time;
        collision.type = preliminary.type;
        collision.outVelocity = outVel;
        collision.position = collisionPos;
        return collision;
    }

    double getRestitution(CollisionType type) {
        switch(type){
            case CollisionType::OUTER_WALL:
            case CollisionType::GOAL_OUTSIDE:
            case CollisionType::GOAL_INSIDE:
                return 0.75;
            case CollisionType::YELLOW_BOT_FRONT:
                break;
            case CollisionType::BLUE_BOT_FRONT:
                break;
            case CollisionType::YELLOW_BOT_HULL:
                break;
            case CollisionType::BLUE_BOT_HULL:
                break;
        }
    }

}