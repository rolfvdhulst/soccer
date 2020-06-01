//
// Created by rolf on 31-05-20.
//

#ifndef SOCCER_FILTERCONSTANTS_H
#define SOCCER_FILTERCONSTANTS_H
//ROBOT FILTER CONSTANTS
//Position: Typical ranges for robot speeds: [-5.0] to [5.0] m/s and

static constexpr double ROBOT_POSITION_INITIAL_COV = 0.1; //[m] Uncertainty in initial robot position
static constexpr double ROBOT_VELOCITY_INITIAL_COV = 4.0; //[m/s] Uncertainty in initial robot velocity (which is 0 for new robots)
static constexpr double ROBOT_POSITION_MEASUREMENT_ERROR = 0.005; //[m] Position uncertainty in robot detections
static constexpr double ROBOT_POSITION_MODEL_ERROR = 4.0 ; // [m/s^2] Assumed white noise in acceleration of a robot for process error

//Angle: Typical range for angles: -10.0 to 10.0 [rad/s]
static constexpr double ROBOT_ANGLE_INITIAL_COV = 0.20;// [rad] 11.5 degrees roughly
static constexpr double ROBOT_ANGULAR_VEL_INITIAL_COV = 5.0; //[rad/s] Uncertainty in initial w
static constexpr double ROBOT_ANGLE_MEASUREMENT_ERROR = 0.02 ; //[rad] 1.1 degrees roughly
static constexpr double ROBOT_ANGLE_MODEL_ERROR =  4.0;//[rad/s^2] Assumed white noise in angular acceleration of a robot

//BALL FILTER CONSTANTS

//static constexpr double BALL_POSITION_MEASUREMENT_ERROR;


#endif //SOCCER_FILTERCONSTANTS_H
