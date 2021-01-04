//
// Created by rolf on 15-09-20.
//

#ifndef SOCCER_VIRTUALBALLTRACKER_H
#define SOCCER_VIRTUALBALLTRACKER_H

#include <vector>
#include <vision/FilteredRobot.h>
#include <vision/DetectionFrame.h>
#include <field/Camera.h>
#include <world/RobotParameters.h> //TODO: refactor robot relevant things (e.g. RobotID, RobotParameters etc.) to a library and remove world from library again
#include <protobuf/WorldVirtualBall.pb.h>

class VirtualBallTracker {
public:

    struct RobotDribblingBall {
        RobotDribblingBall() = default;
        RobotDribblingBall(int robotId, bool botIsBlue) : robotID{robotId}, isBlue{botIsBlue},
                                                          ticksShouldHaveBeenSeen{0}, ticksActuallySeen{0} {}
        int robotID = -1;
        bool isBlue = false;
        int ticksShouldHaveBeenSeen = 0;
        int ticksActuallySeen = 0;
    };
    struct VirtualBall{
        proto::WorldVirtualBall asProto() const;
        RobotDribblingBall ball;
        double x;
        double y;
    };

    void generateVirtualBalls(const std::vector<FilteredRobot> &robots,
                              const BallObservation &lastObservation);

    void clearVirtualBalls();

    void updateVirtualBalls(const std::vector<FilteredRobot> &robots, const Camera &camera,
                            const RobotParameters &parameters, const DetectionFrame &frame);

    [[nodiscard]] std::vector<VirtualBall> getViableVirtualBalls(const std::vector<FilteredRobot>& goodRobots, const RobotParameters& parameters) const;

private:
    [[nodiscard]] Eigen::Vector3d
    getDribbledBallPos(const FilteredRobot &robot, const RobotParameters &parameters) const;

    [[nodiscard]] bool ballShouldBeVisible(const Eigen::Vector3d &ballPos, const std::vector<FilteredRobot> &robots,
                                           const Camera &camera,const RobotParameters& parameters) const;

    std::vector<RobotDribblingBall> virtualBalls;
};


#endif //SOCCER_VIRTUALBALLTRACKER_H
