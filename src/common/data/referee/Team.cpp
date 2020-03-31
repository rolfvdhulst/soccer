//
// Created by rolf on 28-03-20.
//

#include "Team.h"

Team::operator Value() const {
    return value;
}
constexpr bool Team::operator==(Team other) const {
    return value == other.value;
}
constexpr bool Team::operator!=(Team other) const {
    return value != other.value;
}
Team::Team() :value(Team::UNKNOWN){ }
std::string Team::toString() const {
    switch(value){
    case YELLOW: return "Yellow";
    case BLUE: return "Blue";
    case UNKNOWN:
    default:
        return "Unknown Team";
    }
}
Team Team::inverse() const {
    switch(value){
    case YELLOW: return Team(proto::BLUE);
    case BLUE: return Team(proto::YELLOW);
    case UNKNOWN:
    default: return Team(proto::UNKNOWN);
    }
}