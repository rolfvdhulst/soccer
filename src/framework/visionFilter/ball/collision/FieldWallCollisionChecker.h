//
// Created by rolf on 11-08-20.
//

#ifndef SOCCER_FIELDWALLCOLLISIONCHECKER_H
#define SOCCER_FIELDWALLCOLLISIONCHECKER_H



#include <math/geometry/Vector2.h>
#include <core/Time.h>
#include <field/GeometryData.h>

/**
 * This namespace contains methods for checking if collisions occur
 * between the ball and the outer field walls and the goalWall
 */
namespace FieldWallCollisionChecker {
    struct SimpleBallSegment{
        Vector2 beforePos;
        Vector2 afterPos;
        Vector2 velocity;
        Time beforeTime;
        Time afterTime;
    };
    struct FieldCollisionResult{
        Vector2 ballCollisionPos;
        Vector2 outVelocity;
        Time collisionTime;
    };
    /**
     * Computes a collision with the outer walls of the field, if one exists
     * @param ballSegment
     * @return
     */
    std::optional<FieldCollisionResult> getFieldOutsideWallCollision(const SimpleBallSegment& ballSegment, const GeometryData& geometryData);

    std::optional<FieldCollisionResult> getFieldGoalWallCollision(const SimpleBallSegment& ballSegment, const GeometryData& geometryData);

};


#endif //SOCCER_FIELDWALLCOLLISIONCHECKER_H
