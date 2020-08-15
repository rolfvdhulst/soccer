//
// Created by rolf on 11-08-20.
//

#include "FieldWallCollisionChecker.h"
#include <math/geometry/LineSegment.h>
#include <math/geometry/Line.h>

namespace FieldWallCollisionChecker{
    std::optional<FieldCollisionResult> getFieldOutsideWallCollision(const SimpleBallSegment& ballSegment, const GeometryData& geometryData){
        //TODO: add check if line is completely within field without margin. If so, simply return nullopt.
        LineSegment ballPath(ballSegment.beforePos,ballSegment.afterPos);
        const double ballRadius = 0.021333; //TODO: fix this being local
        //Get outside wall lines where the ball would collide
        Rectangle outerWall = geometryData.field.getFieldCorrected(geometryData.field.getBoundaryWidth()-ballRadius);
        if(!outerWall.contains(ballSegment.beforePos)){ //check if the ball is inside the collision margin.
            //If we don't do this, sometimes the ball after a collision will be seen as outside of the field and collide with the outside again
            // because it's velocity points into the field
            return std::nullopt;
        }
        std::optional<Vector2> firstCollisionPoint = std::nullopt;
        std::optional<Line> normalThroughCollisionPoint = std::nullopt;
        for(const auto & wallLine: outerWall.lines()){
            std::optional<Vector2> collisionPoint = ballPath.intersects(wallLine);
            if(collisionPoint){
                //If the collision point is closer or does not exist, we register it as the best point
                if(firstCollisionPoint && (*firstCollisionPoint - ballSegment.beforePos).length2() < (*collisionPoint - ballSegment.beforePos).length2()){

                }else{
                    firstCollisionPoint = collisionPoint;
                    //Construct the normal through the collision point
                    Vector2 dir = wallLine.direction();
                    Vector2 normalDir(dir.y(),-dir.x());
                    normalThroughCollisionPoint = Line(*firstCollisionPoint,*firstCollisionPoint+normalDir);
                }
            }
        }
        if(!firstCollisionPoint){
            return std::nullopt;
        }
        FieldCollisionResult result;
        result.ballCollisionPos = *firstCollisionPoint;
        double fractionOfSegment = (ballSegment.beforePos -*firstCollisionPoint).length() / ballPath.length();
        result.collisionTime = ballSegment.beforeTime + Time(fractionOfSegment * (ballSegment.afterTime - ballSegment.beforeTime).asSeconds());
        //TODO: fix this being local?

        //DemoField is roughly 0.6~ 0.9 (estimated 0.75)
        const double restitution= 0.75; //restitution coefficient, fraction of the initial velocity

        // It's also possible to calculate the reflection in the normal  it with trigonometric functions, but those are likely a LOT slower
        //Calculate the normal through the collision point of the lineSegment
        Vector2 normalProjection = normalThroughCollisionPoint->project(ballSegment.beforePos);
        Vector2 reflectedStartPos = normalProjection * 2 - ballSegment.beforePos; // A +(B-A)*2 = B*2 -A
        Vector2 outVel = (reflectedStartPos-*firstCollisionPoint).stretchToLength(restitution*ballSegment.velocity.length()); //In this direction
        result.outVelocity = outVel;
        return result;
    }

    std::optional<FieldCollisionResult> getFieldGoalWallCollision(const SimpleBallSegment& ballSegment, const GeometryData& geometryData){
        LineSegment ballPath(ballSegment.beforePos,ballSegment.afterPos);

        const double ballRadius = 0.021333; //TODO: fix this being local

        //For both goals
        //Check if line and bounding box overlap (extend bounding box by ball radius)
        //If not, return nullopt for this goal
        //else, check for each line of the goal box extended by ball radius if they intersect
    }

}