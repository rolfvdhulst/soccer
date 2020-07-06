//
// Created by rolf on 31-05-20.
//

#ifndef SOCCER_ROBOTORIENTATIONFILTER_H
#define SOCCER_ROBOTORIENTATIONFILTER_H

#include <math/filters/PosVelFilter1D.h>

//Note the getState() function returns invalid results for this class!
class RobotOrientationFilter : public PosVelFilter1D {
public:
    RobotOrientationFilter() : PosVelFilter1D(){

    }
    RobotOrientationFilter(const Eigen::Vector2d &initialState,
                           const Eigen::Matrix2d &initialCovariance,
                           double modelError,
                           double measurementError,
                           const Time &timeStamp);
    void update(const double& position) override;
    [[nodiscard]] double getPosition() const override;
    [[nodiscard]] double getPositionEstimate(const Time &time) const override;
    [[nodiscard]] static double limitAngle(double angle);
};


#endif //SOCCER_ROBOTORIENTATIONFILTER_H
