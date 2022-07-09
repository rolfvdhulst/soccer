//
// Created by rolf on 19-07-20.
//

#include "ball/kick/estimation/chip/ChipFitResult.h"

ChipFitResult::ChipFitResult(Eigen::Vector3d position, Eigen::Vector3d velocity, Time time) : state(ChipState(position,velocity,time)){
}
