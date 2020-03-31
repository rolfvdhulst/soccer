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

constexpr bool GameStage::hasTime() const {
    return value == NORMAL_FIRST_HALF ||
            value == NORMAL_SECOND_HALF ||
            value == EXTRA_TIME_BREAK ||
            value == EXTRA_FIRST_HALF ||
            value == EXTRA_SECOND_HALF ||
            value == PENALTY_SHOOTOUT_BREAK;
}
GameStage::GameStage() : value(NORMAL_FIRST_HALF_PRE){}
GameStage::GameStage(const proto::Referee::Stage& gameStage) {
    switch(gameStage){
    case proto::Referee_Stage_NORMAL_FIRST_HALF_PRE: value = NORMAL_FIRST_HALF_PRE; return;
    case proto::Referee_Stage_NORMAL_FIRST_HALF: value = NORMAL_FIRST_HALF; return;
    case proto::Referee_Stage_NORMAL_HALF_TIME: value = NORMAL_HALF_TIME; return;
    case proto::Referee_Stage_NORMAL_SECOND_HALF_PRE: value = NORMAL_SECOND_HALF_PRE; return;
    case proto::Referee_Stage_NORMAL_SECOND_HALF: value = NORMAL_SECOND_HALF; return;
    case proto::Referee_Stage_EXTRA_TIME_BREAK: value = EXTRA_TIME_BREAK; return;
    case proto::Referee_Stage_EXTRA_FIRST_HALF_PRE: value = EXTRA_FIRST_HALF_PRE; return;
    case proto::Referee_Stage_EXTRA_FIRST_HALF: value = EXTRA_FIRST_HALF; return;
    case proto::Referee_Stage_EXTRA_HALF_TIME: value = EXTRA_HALF_TIME; return;
    case proto::Referee_Stage_EXTRA_SECOND_HALF_PRE: value = EXTRA_SECOND_HALF_PRE; return;
    case proto::Referee_Stage_EXTRA_SECOND_HALF: value = EXTRA_SECOND_HALF; return;
    case proto::Referee_Stage_PENALTY_SHOOTOUT_BREAK: value = PENALTY_SHOOTOUT_BREAK; return;
    case proto::Referee_Stage_PENALTY_SHOOTOUT: value = PENALTY_SHOOTOUT; return;
    case proto::Referee_Stage_POST_GAME: value = POST_GAME; return;
    }

}
std::string GameStage::toString() const {
    switch(value){
    case NORMAL_FIRST_HALF_PRE:return"NORMAL_FIRST_HALF_PRE";
    case NORMAL_FIRST_HALF:return"NORMAL_FIRST_HALF";
    case NORMAL_HALF_TIME:return"NORMAL_HALF_TIME";
    case NORMAL_SECOND_HALF_PRE:return"NORMAL_SECOND_HALF_PRE";
    case NORMAL_SECOND_HALF:return"NORMAL_SECOND_HALF";
    case EXTRA_TIME_BREAK:return"EXTRA_TIME_BREAK";
    case EXTRA_FIRST_HALF_PRE:return"EXTRA_FIRST_HALF_PRE";
    case EXTRA_FIRST_HALF:return"EXTRA_FIRST_HALF";
    case EXTRA_HALF_TIME:return"EXTRA_HALF_TIME";
    case EXTRA_SECOND_HALF_PRE:return"EXTRA_SECOND_HALF_PRE";
    case EXTRA_SECOND_HALF:return"EXTRA_SECOND_HALF";
    case PENALTY_SHOOTOUT_BREAK:return"PENALTY_SHOOTOUT_BREAK";
    case PENALTY_SHOOTOUT:return"PENALTY_SHOOTOUT";
    case POST_GAME:return"POST_GAME";
    }
}
