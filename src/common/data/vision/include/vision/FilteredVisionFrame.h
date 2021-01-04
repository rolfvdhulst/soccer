//
// Created by rolf on 05-08-20.
//

#ifndef SOCCER_FILTEREDVISIONFRAME_H
#define SOCCER_FILTEREDVISIONFRAME_H
#include "FilteredBall.h"
#include "FilteredRobot.h"
struct FilteredVisionFrame{
    std::optional<FilteredBall> ball;
    std::vector<FilteredRobot> blueRobots;
    std::vector<FilteredRobot> yellowRobots;
};
#endif //SOCCER_FILTEREDVISIONFRAME_H
