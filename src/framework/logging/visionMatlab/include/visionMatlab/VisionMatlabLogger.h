//
// Created by rolf on 03-06-20.
//

#ifndef SOCCER_VISIONMATLABLOGGER_H
#define SOCCER_VISIONMATLABLOGGER_H

#include <QString>
#include <iostream>
#include <fstream>
#include <Eigen/Dense>


class VisionMatlabLogger {
public:
    enum FilterType{
        BALL_FILTER = 0,
        ROBOT_FILTER_POSITION_BLUE = 1,
        ROBOT_FILTER_POSITION_YELLOW = 2,
        ROBOT_FILTER_ANGLE_BLUE = 3,
        ROBOT_FILTER_ANGLE_YELLOW = 4
    };
    void startLogging();
    void endLogging();
    bool isCurrentlyLogging() const;
    bool openFile(QString fileName);
    int writeNewFilter(int StateDim, int ObsDim, FilterType type);
    void writeData(int id, double time, const Eigen::VectorXd& observation,const Eigen::VectorXd& state,const Eigen::MatrixXd &covariance, const Eigen::VectorXd& innovation);
    void closeFile();
private:
    int nextFilterID = 1; //matlab indices start at 1 ...

    std::ofstream* outStream;
    bool isLogging = false;
};

//I know, global variable, but this or a singleton is required to run nicely and it scales better If we would ever do things such as run 2 worlds etc.
namespace matlab_logger{
    extern VisionMatlabLogger logger;
}
#endif //SOCCER_VISIONMATLABLOGGER_H
