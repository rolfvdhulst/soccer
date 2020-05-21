//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_BALLSTATE_H
#define SOCCER_BALLSTATE_H

#include <core/Time.h>
#include <math/geometry/Vector2.h>
#include <math/geometry/Vector3.h>
#include <protobuf/WorldBall.pb.h>

class BallState {
    public:
        explicit BallState(const proto::WorldBall& ball);
        [[nodiscard]] const Vector2& pos() const;
        [[nodiscard]] const Vector2& vel() const;
        [[nodiscard]] bool isVisible() const;
        [[nodiscard]] double radius() const;
    private:
        Vector2 _position;
        Vector2 _velocity;
        bool _isVisible;
};


#endif  // SOCCER_BALLSTATE_H
