//
// Created by rolf on 19-03-20.
//

#include "GameStage.h"
constexpr GameStage::GameStage(GameStage::Stage stage) : value(stage){
}
GameStage::operator Stage() const {
    return value;
}
constexpr bool GameStage::operator==(GameStage other) const {
    return value == other.value;
}
constexpr bool GameStage::operator!=(GameStage other) const {
    return value != other.value;
}
constexpr bool GameStage::isPreHalf() const {
    return value == NORMAL_FIRST_HALF_PRE ||
        value == NORMAL_SECOND_HALF_PRE ||
        value == EXTRA_FIRST_HALF_PRE ||
        value == EXTRA_SECOND_HALF_PRE;
}
constexpr bool GameStage::isNormalPlayTime() const {
    return value == NORMAL_FIRST_HALF ||
        value == NORMAL_SECOND_HALF;
}
constexpr bool GameStage::isBreak() const {
    return value == NORMAL_HALF_TIME ||
        value == EXTRA_TIME_BREAK ||
        value == EXTRA_HALF_TIME ||
        value == PENALTY_SHOOTOUT_BREAK;
}
constexpr bool GameStage::isExtendedPlayTime() const {
    return value == EXTRA_FIRST_HALF ||
        value == EXTRA_SECOND_HALF;
}
