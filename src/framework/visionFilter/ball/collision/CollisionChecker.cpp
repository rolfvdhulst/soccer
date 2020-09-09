//
// Created by rolf on 11-08-20.
//

#include "ball/collision/CollisionChecker.h"
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
        //TODO: check these values with geometry from SSL!
        const double ballRadius = 0.021333; //TODO: fix this being local
        const double halfFieldLength = geometryData.field.getFieldLength()*0.5 + 0.5*geometryData.field.getLineThickness();
        const double halfGoalWidth = geometryData.field.getGoalWidth()*0.5;
        const double goalDepth = geometryData.field.getGoalDepth();
        const double goalWallWidth = geometryData.field.getGoalWallThickness();

        std::optional<CollisionPreliminaryResult> bestCollision = std::nullopt;
        //Check both goals
        //TODO: check if goal coordinates correspond to field properly and fill in
        for(double dir : {-1.0, 1.0}){
            //Define goal with a margin
            Vector2 backLeft = Vector2(halfFieldLength+goalDepth+goalWallWidth+ballRadius,halfGoalWidth+goalWallWidth+ballRadius) * dir;
            Vector2 frontRightPostRight = Vector2(halfFieldLength-ballRadius,-(halfGoalWidth+goalWallWidth+ballRadius)) * dir ;
            BoundingBox2D rect(backLeft,frontRightPostRight);
            if(!rect.doesIntersect(ballPath)){//pruning: if the line does not overlap this (which is a much faster check), there's 100% no collisions
                continue;
            };
            Vector2 backRight = Vector2(halfFieldLength+goalDepth+goalWallWidth+ballRadius,-(halfGoalWidth+goalWallWidth+ballRadius)) * dir;
            Vector2 frontRightPostLeft = Vector2(halfFieldLength-ballRadius,-(halfGoalWidth-ballRadius)) * dir ;
            Vector2 insideRight = Vector2(halfFieldLength + goalDepth -ballRadius,-(halfGoalWidth-ballRadius)) * dir ;
            Vector2 insideLeft = Vector2(halfFieldLength + goalDepth -ballRadius,halfGoalWidth-ballRadius) * dir ;
            Vector2 frontLeftPostRight = Vector2(halfFieldLength -ballRadius,halfGoalWidth-ballRadius) * dir ;
            Vector2 frontLeftPostLeft = Vector2(halfFieldLength -ballRadius,halfGoalWidth+goalWallWidth+ballRadius) * dir ;

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
                        Vector2 lineDir = line.direction();
                        result.normalDir =  Vector2(lineDir.y(),-lineDir.x());
                        result.type = (i>0 && i <4) ? CollisionType::GOAL_INSIDE : CollisionType::GOAL_OUTSIDE;
                        //If the collision point is closer or does not exist, we register it as the best point
                        bestCollision = result;
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

        //TODO: make this a constructor
        Collision collision;
        collision.collisionTime = segment.startTime + Time(time);
        collision.dt = time;
        collision.type = preliminary.type;
        collision.inVelocity = collisionVel;
        collision.outVelocity = outVel;
        collision.position = collisionPos;
        return collision;
    }

    std::optional<RobotCollisionPreliminaryResult> checkRobotConstVel(const BallTrajectorySegment& ballSegment, const RobotTrajectorySegment& trajectory){
        //Take the robot as frame of reference, this makes the calculations a bit easier to do:
        //TODO: check if robot and ball dt's are the same
        double dt = ballSegment.dt;
        const double ballRadius = 0.021333;//TODO: fix constant hardcoded
        if(ballSegment.dt != trajectory.dt && trajectory.dt != 0.0){ //If robots are just initialized dt == 0.0 happens occasionally
            //In that case we can still use the calculations, as then the velocities are still estimated ok
            //TODO: in case of a collision ballSegment.dt is adjusted, so we need to check if everything still runs as planned...
            return std::nullopt;
        }

        LineSegment ballPath(ballSegment.startPos,ballSegment.endPos-trajectory.vel*dt);

        BoundingBox2D robotBox = trajectory.startPos.boundingBox();
        robotBox.expand(ballRadius);
        if(!robotBox.doesIntersect(ballPath)){
            return std::nullopt;
        }
        //TODO: add bounding box test? See if this is faster
        Circle robotCircle(trajectory.startPos.pos(),trajectory.startPos.radius()+ballRadius);
        std::vector<double>  intersectTs = robotCircle.intersectionParametrized(ballPath);
        if(intersectTs.empty() && ! robotCircle.contains(ballPath.start())){
            return std::nullopt;
        }
        double circleTime = 0;
        double t = 0;
        if (intersectTs.size() == 1){
            t = intersectTs[0];
            circleTime = solveCollisionTime(ballSegment,trajectory,t);
        }else if(intersectTs.size()>1){
            t = fmin(intersectTs[0],intersectTs[1]);
            circleTime = solveCollisionTime(ballSegment,trajectory,t);
        }
        Angle circleAngle = trajectory.startPos.angle() + trajectory.angVel*circleTime;
        Vector2 initialPoint = ballPath.getPos(t);
        RobotShape circleStartBot(trajectory.startPos.pos(),trajectory.startPos.centerToFrontDist()+ballRadius,
                                  trajectory.startPos.radius()+ballRadius,circleAngle);
        if(circleStartBot.contains(ballSegment.startPos)){
            return std::nullopt;//Ball is already colliding, we don't double count
        }
        if(!circleStartBot.inFrontOfDribbler(initialPoint)){
            //Hull collision. Things are fairly easy
            RobotCollisionPreliminaryResult result;
            result.segment = trajectory;
            result.dt = circleTime;
            result.normalDir = initialPoint-circleStartBot.pos();
            result.type = CollisionType::ROBOT_HULL;
            return result;
        }else{
            double minSearch = circleTime;
            double maxSearch = intersectTs.size() > 1 ? fmax(intersectTs[0],intersectTs[1]) : dt;
            //(Possible) front collision. This involves a formula which has no analytical roots so we do bisection search
            //TODO: fix constructor
            RobotConstVelModel model;
            model.startPos = ballSegment.startPos-trajectory.startPos.pos();
            model.vel = ballSegment.startVel - trajectory.vel;
            model.acc = ballSegment.acceleration;
            model.initialAngle = trajectory.startPos.angle().getAngle();
            model.angVel = trajectory.angVel;
            model.centerToFront = circleStartBot.centerToFrontDist();
            std::optional<double> frontCollisionTime = solveRobotFrontCollisionTime(model,minSearch,maxSearch);
            bool doesRobotKickerLineIntersect = circleStartBot.kicker().doesIntersect(ballPath);
            if(frontCollisionTime){
                RobotCollisionPreliminaryResult result;
                result.segment = trajectory;
                result.dt = frontCollisionTime.value();
                Angle collisionAngle(model.initialAngle+model.angVel*dt);
                result.normalDir = Vector2(collisionAngle);
                result.type = CollisionType::ROBOT_FRONT;
                return result;
            }else{
                return std::nullopt;
            }
        }
    }
    std::optional<double> solveRobotFrontCollisionTime(RobotConstVelModel model, double minSearch, double maxSearch){
        //Note minsearch and maxsearch get changed by this function after copying!
        //First, check if the model has different values on both sides so we can use bisection.
        assert(model.fvalue(minSearch)>=0);
        if(model.fvalue(minSearch)*model.fvalue(maxSearch)<0.0){
            //Bisection method
            double time = (minSearch + maxSearch)*0.5;
            //accuracy is 2^-maxIts in time. 20 iterations gives us more than enough accuracy (10^-6 relative accuracy or so)
            int maxIts = 20;
            for (int n = 0; n < maxIts ; ++n) {
                time = (minSearch + maxSearch)*0.5;
                if(model.fvalue(time)*model.fvalue(minSearch)>=0.0){ //check if signs are opposed or the same
                    minSearch = time;
                }else{
                    maxSearch = time;
                }
            }
            bool converged = model.fvalue(minSearch)*model.fvalue(maxSearch) <= 0.0; //still opposite signs?
            return converged ? std::optional<double>(time) : std::nullopt;
        }else{
            //There's a good chance the ball passes ' in front'  of the robot.
            // Problem is, that if the robot turns fast enough, this might not be the case(e.g. ball 'enters' and leaves robot again
            //This case happens a lot. Usually it happens when a robot is slowly approaching the ball.
            return std::nullopt;
        }
    }
    double solveCollisionTime(const BallTrajectorySegment& ballSegment, const RobotTrajectorySegment& trajectory,
                              double lineFraction){
        double a = 0.5 * ballSegment.acc;
        double b = (ballSegment.startVel-trajectory.vel).length();
        double c = - lineFraction*(b*ballSegment.dt+0.5*ballSegment.acc*ballSegment.dt*ballSegment.dt);
        auto times = solveQuadratic(a,b,c);
        if(times){
            return (times->first<=ballSegment.dt) ? times->first : times->second; //we need the smaller root, which is guaranteed <dt through the collision calculation
        } else{
            return lineFraction*ballSegment.dt; //use a constant velocity model in case quadratic equation somehow fails. This usually is not far off
        }
    }
    Collision robotCollideAndReflect(const RobotCollisionPreliminaryResult &preliminary, const BallTrajectorySegment& segment){
        Vector2 collisionPos = segment.getPosition(preliminary.dt);
        Vector2 collisionVel = segment.getVelocity(preliminary.dt);

        //Determine relative collision velocity between the two contact points
        //We use robot frame of reference for consistency
        Vector2 robotPointVel = segment.startVel;
        //Add rotational component for front collisions. Left out for now because I probably fucked it up
        //Maybe also do so for hull collisions, but that needs better friction estimation between ball and hull
        /**
        if(preliminary.type == CollisionType::BLUE_BOT_FRONT || preliminary.type == CollisionType::YELLOW_BOT_FRONT){
            Vector2 robotPos = preliminary.segment.startPos.pos() + preliminary.segment.vel * preliminary.dt;
            Angle robotAngle = preliminary.segment.startPos.angle() + preliminary.segment.angVel * preliminary.dt;
            RobotShape robotAtCollision(robotPos,preliminary.segment.startPos.centerToFrontDist(),preliminary.segment.startPos.radius(),robotAngle);
            Vector2 robotTouchPos = robotAtCollision.project(collisionPos);//Find the collision position.
            // Alternatively, this could also be find by the collision normal we passed
            //TODO: check if this calculation actually makes sense... lol
            Vector2 arm = robotTouchPos-robotPos;
            double armLength = arm.length();
            Vector2 direction(-arm.y(),arm.x());//Rotated 90 degrees
            Vector2 angularContribution = direction.normalize() * armLength * preliminary.segment.angVel;
            //TODO: maybe cap the above contribution by some max angular vel
            //TODO: check if the above vector is pointing the right direction (e.g. if omega is clockwise the signs are wrong)
            robotPointVel += angularContribution;
        }
         */
        Vector2 relativeVelocity = collisionVel - robotPointVel;
        //mirror velocity in collision normal
        //one can also use angles, but that's probably a lot slower
        Line normalThroughCollision(collisionPos,collisionPos+preliminary.normalDir);
        Vector2 beforeLinePos = (collisionPos-relativeVelocity);
        Vector2 projection =  normalThroughCollision.project(beforeLinePos);
        Vector2 afterLinePos = projection*2.0 - beforeLinePos; // A +(B-A)*2 = B*2 -A

        double restitution = getRestitution(preliminary.type);
        double outVelLength = restitution * relativeVelocity.length();
        Vector2 outVel = (afterLinePos-collisionPos).stretchToLength( outVelLength) + preliminary.segment.vel;//back to world coordinates

        //TODO: make this a constructor
        Collision robot;
        robot.collisionTime = segment.startTime + Time(preliminary.dt);
        robot.dt = preliminary.dt;
        robot.type = preliminary.type;
        robot.inVelocity = collisionVel;
        robot.outVelocity = outVel;
        robot.position = collisionPos;
        robot.robotID = preliminary.segment.robotID;
        robot.robotIsBlue = preliminary.segment.isBlue;
        return robot;
    }
    double getRestitution(CollisionType type) {
        //TODO: these need to be tuned or abstracted away through some other way
        switch(type){
            case CollisionType::OUTER_WALL:
            case CollisionType::GOAL_OUTSIDE:
            case CollisionType::GOAL_INSIDE:
                return 0.75;
            case CollisionType::ROBOT_FRONT:
                return 0.3;//TODO: figure out this number
            case CollisionType::ROBOT_HULL:
                return 0.6; //TODO: figure out this number
            default:
                std::cerr<<"COULD NOT FIND COLLISION TYPE in resititution table!"<<std::endl;
                return 0.75;
        }
    }

    double RobotConstVelModel::fvalue(double t) const noexcept{
        Vector2 ballPos = startPos + vel * t + acc*0.5*t*t;
        double theta = initialAngle + angVel*t;
        double value =ballPos.x()*cos(theta)+ ballPos.y()*sin(theta) - centerToFront;
        return value;//signed distance to robotLine scaled by a factor
    }

    double RobotConstVelModel::derivative(double t) const noexcept {
        Vector2 ballPos = startPos + vel * t + acc*0.5*t*t;
        Vector2 ballSpeed = vel + acc*t;
        double theta = initialAngle + angVel*t;
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);
        return (ballSpeed.x() + angVel*ballPos.y())*cosTheta + (ballSpeed.y()-angVel*ballPos.x())*sinTheta;
    }

    std::pair<double, double> RobotConstVelModel::funcAndDerivative(double t) const noexcept {
        Vector2 ballPos = startPos + vel * t + acc*0.5*t*t;
        Vector2 ballSpeed = vel + acc*t;
        double theta = initialAngle + angVel*t;
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);
        double value = ballPos.x()*cosTheta + ballPos.y()*sinTheta - centerToFront;
        double derivative = (ballSpeed.x() + angVel*ballPos.y())*cosTheta + (ballSpeed.y()-angVel*ballPos.x())*sinTheta;
        return std::make_pair(value,derivative);
    }
}