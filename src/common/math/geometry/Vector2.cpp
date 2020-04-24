#include "geometry/Vector2.h"
#include "geometry/Angle.h"
#include "geometry/Definitions.h"

Vector2::Vector2(Angle &angle, const double &length) : m_x{cos(angle.getAngle()) * length}, m_y{sin(angle.getAngle()) * length} {}

double &Vector2::x() { return m_x; }
double &Vector2::y() { return m_y; }
const double &Vector2::x() const { return m_x; }
const double &Vector2::y() const { return m_y; }
double Vector2::dot(const Vector2 &other) const { return this->m_x * other.m_x + this->m_y * other.m_y; }

double Vector2::dist(const Vector2 &other) const { return sqrt(dist2(other)); }

double Vector2::dist2(const Vector2 &other) const { return (*this - other).length2(); }

Vector2 Vector2::scale(double scalar) const { return {m_x * scalar, m_y * scalar}; }

Vector2 Vector2::normalize() const {
    if (this->length() == 0.0) return {0.0, 0.0};

    double d = 1.0 / length();
    return {m_x * d, m_y * d};
}

double Vector2::length() const { return sqrt(this->length2()); }

double Vector2::length2() const { return (m_x * m_x + m_y * m_y); }

double Vector2::angle() const { return this->toAngle().getAngle(); }

Angle Vector2::toAngle() const { return Angle(atan2(m_y, m_x)); }

Vector2 Vector2::lerp(const Vector2 &other, double factor) const { return this->scale(factor) + other.scale(1 - factor); }

Vector2 Vector2::rotate(double radians) const {
    double c = cos(radians);
    double s = sin(radians);
    return Vector2(m_x * c - m_y * s, m_x * s + m_y * c);
}

Vector2 Vector2::project(const Vector2 &lineA, const Vector2 &lineB) const {
    Vector2 ab = lineB - lineA;
    Vector2 ap = *this - lineA;
    double t = ap.dot(ab) / ab.dot(ab);
    if (t < 0) {
        return lineA;
    } else if (t > 1) {
        return lineB;
    }
    return lineA + ab.scale(t);
}

Vector2 Vector2::project2(const Vector2 &ab) const { return ab.scale(this->dot(ab) / ab.dot(ab)); }

bool Vector2::isNotNaN() const {
    /**
     * NaN is defined as not being equal to itself
     */
    return m_x == m_x && m_y == m_y;
}

Vector2 Vector2::stretchToLength(double desiredLength) const {
    if (length() == 0.0) {
        return {desiredLength, 0};
    }
    double frac = desiredLength / length();
    return {m_x * frac, m_y * frac};
}
Vector2 Vector2::abs() const { return {std::abs(m_x), std::abs(m_y)}; }

double Vector2::cross(const Vector2 &other) const { return this->m_x * other.m_y - this->m_y * other.m_x; }

bool Vector2::operator==(const Vector2 &other) const { return this->m_x == other.m_x && this->m_y == other.m_y; }
bool Vector2::approx(const Vector2 &other) const { return fabs(this->m_x - other.m_x) < VECTOR_PRECISION && fabs(this->m_y - other.m_y) < VECTOR_PRECISION; }

bool Vector2::operator!=(const Vector2 &other) const { return !(*this == other); }

bool Vector2::operator<(const Vector2 &other) const { return this->length() < other.length(); }

Vector2 Vector2::operator+=(const Vector2 &other) { return {this->m_x += other.m_x, this->m_y += other.m_y}; }

Vector2 Vector2::operator+=(const double &scalar) { return {m_x += scalar, m_y += scalar}; }

Vector2 Vector2::operator-=(const Vector2 &other) { return {this->m_x -= other.m_x, this->m_y -= other.m_y}; }

Vector2 Vector2::operator-=(const double &scalar) { return {m_x -= scalar, m_y -= scalar}; }

Vector2 Vector2::operator*=(const Vector2 &other) { return {this->m_x *= other.m_x, this->m_y *= other.m_y}; }

Vector2 Vector2::operator*=(const double &scalar) { return {m_x *= scalar, m_y *= scalar}; }

Vector2 Vector2::operator/=(const Vector2 &other) {
    assert(!(other == Vector2()) && "Division by zero");
    return {this->m_x /= other.m_x, this->m_y /= other.m_y};
}

Vector2 Vector2::operator/=(const double &scalar) { return {m_x /= scalar, m_y /= scalar}; }

Vector2 Vector2::operator+(const Vector2 &other) const { return {m_x + other.m_x, m_y + other.m_y}; }

Vector2 Vector2::operator+(const double &scalar) const { return {m_x + scalar, m_y + scalar}; }

Vector2 Vector2::operator-(const Vector2 &other) const { return {this->m_x - other.m_x, this->m_y - other.m_y}; }

Vector2 Vector2::operator-(const double &scalar) const { return {m_x - scalar, m_y - scalar}; }

Vector2 Vector2::operator*(const Vector2 &other) const { return {this->m_x * other.m_x, this->m_y * other.m_y}; }

Vector2 Vector2::operator*(const double &scalar) const { return {m_x * scalar, m_y * scalar}; }

Vector2 Vector2::operator/(const Vector2 &other) const { return {this->m_x / other.m_x, this->m_y / other.m_y}; }

Vector2 Vector2::operator/(const double &scalar) const { return {m_x / scalar, m_y / scalar}; }

Vector2 &Vector2::operator=(const proto::Vector2f &msg) {
    m_x = msg.x();
    m_y = msg.y();
    return *this;
}

Vector2::operator proto::Vector2f() const {
    proto::Vector2f msg;
    msg.set_x(m_x);
    msg.set_y(m_y);
    return msg;
}

std::ostream &Vector2::write(std::ostream &os) const { return os << "{ x = " << m_x << ", y = " << m_y << " }"; }

std::ostream &operator<<(std::ostream &os, Vector2 const &vec) { return vec.write(os); }
