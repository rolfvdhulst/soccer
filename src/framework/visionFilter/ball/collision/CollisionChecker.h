//
// Created by rolf on 11-08-20.
//

#ifndef SOCCER_COLLISIONCHECKER_H
#define SOCCER_COLLISIONCHECKER_H



#include "ball/BallTrajectorySegment.h"
#include <field/GeometryData.h>

/**
 * This namespace contains methods for checking if collisions occur
 * between the ball and the outer field walls and the goalWall
 */
namespace CollisionChecker {

    enum class CollisionType{
        OUTER_WALL= 0,
        GOAL_OUTSIDE= 1,
        GOAL_INSIDE = 2,
        YELLOW_BOT_FRONT = 3,
        YELLOW_BOT_HULL = 4,
        BLUE_BOT_FRONT = 5,
        BLUE_BOT_HULL = 6
    };

    struct CollisionPreliminaryResult{
        Vector2 normalDir; //This simply gives the line the normal lies on, it can be flipped by 90 degrees!
        double distanceFraction;
        CollisionType type;
    };

    struct Collision{
        Vector2 position;
        Vector2 outVelocity;
        double dt;
        Time collisionTime;
        CollisionType type;
    };

    struct RobotCollisionPreliminaryResult{
        CollisionPreliminaryResult collision;

    };
    /**
     * Computes a collision with the outer walls of the field, if one exists
     * @param ballSegment
     * @return
     */
    std::optional<CollisionPreliminaryResult> getFieldOutsideWallCollision(const BallTrajectorySegment& ballSegment, const GeometryData& geometryData);

    std::optional<CollisionPreliminaryResult> getFieldGoalWallCollision(const BallTrajectorySegment& ballSegment, const GeometryData& geometryData);

    Collision fieldCollideAndReflect(const BallTrajectorySegment& segment, const CollisionPreliminaryResult& preliminary);
};


#endif //SOCCER_COLLISIONCHECKER_H
