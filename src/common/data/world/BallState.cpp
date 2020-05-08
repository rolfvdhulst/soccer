//
// Created by rolf on 23-2-20.
//

#include "BallState.h"

const Vector2& BallState::pos() const {
    return _position;
}
const Vector2& BallState::vel() const {
    return _velocity;
}
bool BallState::isVisible() const {
    return _isVisible;
}
