//
// Created by rolf on 27-03-20.
//

#include "GameCommand.h"
constexpr bool GameCommand::operator==(GameCommand other) const {
    return value == other.value;
}
constexpr bool GameCommand::operator!=(GameCommand other) const {
    return value != other.value;
}
GameCommand::operator Command() const {
    return value;
}
constexpr GameCommand::GameCommand(GameCommand::Command command) :value(command){ }
