//
// Created by rolf on 23-2-20.
//

#include <protobuf/WorldBall.pb.h>
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
BallState::BallState(const proto::WorldBall &ball) :
_position{ball.pos()},
_velocity{ball.vel()},
_isVisible{ball.visible()}{

}
