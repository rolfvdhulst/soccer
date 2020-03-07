//
// Created by rolf on 28-2-20.
//

#include "Rotation.h"
bool Rotation::operator==(const Rotation &other) const {
    return fabs(rotation-other.rotation)<VECTOR_PRECISION;
}
bool Rotation::operator==(const double &scalar) const {
    return fabs(rotation - scalar)<VECTOR_PRECISION;
}
bool Rotation::operator!=(const Rotation &other) const {
    return !(*this == other);
}
bool Rotation::operator!=(const double &scalar) const {
    return !(*this == scalar);
}
Rotation Rotation::operator+(const Rotation &other) const {
    return Rotation(rotation+other.rotation);
}
Rotation::Rotation(double rotation) :rotation{rotation} {
}
Rotation Rotation::operator+(const double &scalar) const {
    return Rotation(rotation+scalar);
}
Rotation Rotation::operator-(const Rotation &other) const {
    return Rotation(rotation-other.rotation);
}
Rotation Rotation::operator-(const double &scalar) const {
    return Rotation(rotation-scalar);
}
Rotation Rotation::operator+=(const Rotation &other) {
    return Rotation(rotation+=other.rotation);
}
Rotation Rotation::operator+=(const double &scalar) {
    return Rotation(rotation+=scalar);
}
Rotation Rotation::operator-=(const Rotation &other) {
    return Rotation(rotation-=other.rotation);
}
Rotation Rotation::operator-=(const double &scalar) {
    return Rotation(rotation-=scalar);
}
Rotation::operator double() const {
    return rotation;
}
double Rotation::getRotation() const {
    return rotation;
}
void Rotation::setRotation(double rot) {
    rotation=rot;
}

