//
// Created by rolf on 11-08-20.
//

#ifndef SOCCER_COLLISIONCHECKER_H
#define SOCCER_COLLISIONCHECKER_H



#include "ball/BallTrajectorySegment.h"
#include "RobotTrajectorySegment.h"
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
        ROBOT_FRONT = 3,
        ROBOT_HULL = 4
    };

    struct CollisionPreliminaryResult{
        Vector2 normalDir; //This simply gives the line the normal lies on, it can be flipped by 90 degrees!
        double distanceFraction;
        CollisionType type;
    };

    struct Collision{
        Vector2 position;
        Vector2 inVelocity;
        Vector2 outVelocity;
        double dt;
        Time collisionTime;
        CollisionType type;
        int robotID = -1;
        bool robotIsBlue = false;
    };

    struct RobotCollisionPreliminaryResult{
        double dt;
        Vector2 normalDir;
        CollisionType type;
        RobotTrajectorySegment segment;
    };


    struct RobotConstVelModel{
        //assumes the robot is at (0,0), and the ball velocity is given in the robots frame of reference
        Vector2 startPos;
        Vector2 vel;
        Vector2 acc;
        double initialAngle;
        double angVel;
        double centerToFront;
        [[nodiscard]] double fvalue(double t) const noexcept;
        [[nodiscard]] double derivative(double t) const noexcept;
        [[nodiscard]] std::pair<double,double> funcAndDerivative(double t) const noexcept;
    };
    /**
     * Computes a collision with the outer walls of the field, if one exists
     * @param ballSegment
     * @return
     */
    std::optional<CollisionPreliminaryResult> getFieldOutsideWallCollision(const BallTrajectorySegment& ballSegment, const GeometryData& geometryData);

    std::optional<CollisionPreliminaryResult> getFieldGoalWallCollision(const BallTrajectorySegment& ballSegment, const GeometryData& geometryData);

    Collision fieldCollideAndReflect(const BallTrajectorySegment& segment, const CollisionPreliminaryResult& preliminary);

    std::optional<RobotCollisionPreliminaryResult> checkRobotConstVel(const BallTrajectorySegment& ballSegment, const RobotTrajectorySegment& trajectory);
    std::optional<double> solveRobotFrontCollisionTime(RobotConstVelModel model, double minSearch, double maxSearch);
    double solveCollisionTime(const BallTrajectorySegment& ballSegment, const RobotTrajectorySegment& trajectory,
                              double lineFraction);
    Collision robotCollideAndReflect(const RobotCollisionPreliminaryResult &preliminary, const BallTrajectorySegment& segment);

    std::optional<CollisionPreliminaryResult> checkRobotOuterCollision(const BallTrajectorySegment& ballSegment);

    std::optional<CollisionPreliminaryResult> checkRobotFrontCollision(const BallTrajectorySegment& ballSegment);

    double getRestitution(CollisionType type);
};


#endif //SOCCER_COLLISIONCHECKER_H
