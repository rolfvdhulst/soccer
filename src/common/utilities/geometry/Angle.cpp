//
// Created by thijs on 28-2-19.
//

#include "Angle.h"
#include "Vector2.h"

Angle::Angle(double angle)
        :angle(angle) { this->constrain(); }

Angle::Angle(const Vector2 &vec) {
    if (vec.length() < VECTOR_PRECISION){
        angle = 0.0;
    }
    else{
        angle = vec.toAngle().getAngle();
    }
}

double Angle::getAngle() const {
    return angle;
}

void Angle::setAngle(double other) {
    *this = Angle(other);
}

Angle Angle::constrain() {
    this->angle = fmod(angle + M_PI, 2*M_PI);
    this->angle = (angle < 0) ? angle + M_PI : angle - M_PI;
    return *this;
}

double Angle::angleDiff(Angle const &other) const {
    return (*this - other).angle;
}

double Angle::angleDiff(double scalar) const {
    return (*this-scalar).angle;
}

Vector2 Angle::toVector2(double length) const {
    Vector2 vec = Vector2(cos(this->angle), sin(this->angle));
    return vec.stretchToLength(length);
}

bool Angle::operator==(const Angle &other) const {
    return fabs(Angle(this->angle - other.angle).angle) < VECTOR_PRECISION;
}

bool Angle::operator==(const double &scalar) const {
    return *this == Angle(scalar);
}

bool Angle::operator!=(const Angle &other) const {
    return ! (*this == other);
}

bool Angle::operator!=(const double &scalar) const {
    return *this != Angle(scalar);
}


Angle Angle::operator+(const Angle &other) const {
    return Angle(this->angle + other.angle);
}

Angle Angle::operator+(const double &scalar) const {
    return Angle(this->angle + scalar);
}

Angle Angle::operator-(const Angle &other) const {
    return Angle(this->angle - other.angle);
}

Angle Angle::operator-(const double &scalar) const {
    return Angle(this->angle - scalar);
}

Angle Angle::operator+=(const Angle &other) {
    this->angle += other.angle;
    this->constrain();
    return *this;
}

Angle Angle::operator+=(const double &scalar) {
    this->angle += scalar;
    this->constrain();
    return *this;
}

Angle Angle::operator-=(const Angle &other) {
    this->angle -= other.angle;
    this->constrain();
    return *this;
}

Angle Angle::operator-=(const double &scalar) {
    this->angle -= scalar;
    this->constrain();
    return *this;
}

Angle &Angle::operator=(const double &scalar) {
    this->angle = scalar;
    this->constrain();
    return *this;
}

Angle::operator double() const {
    return this->angle;
}
