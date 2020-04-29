//
// Created by rolf on 24-04-20.
//

#include "geometry/LineBase.h"

double LineBase::slope() const { return (m_end.y() - m_start.y()) / (m_end.x() - m_start.x()); }

bool LineBase::isVertical() const { return (m_end.x() == m_start.x()) && (m_end.y() != m_start.y()); }

Vector2 LineBase::direction() const { return m_end - m_start; }

double LineBase::intercept() const { return m_start.y() - this->slope() * m_start.x(); }

std::pair<double, double> LineBase::coefficients() const { return {slope(), intercept()}; }

bool LineBase::isPoint() const { return m_start == m_end; }

bool LineBase::isParallel(const LineBase &other) const {
    // check if line is vertical, otherwise check the slopes
    bool bothVertical = this->isVertical() && other.isVertical();
    if (bothVertical) {
        return true;
    }
    return this->slope() == other.slope();
}
bool LineBase::isHorizontal() const { return (m_start.y() == m_end.y()) && (m_start.x() != m_end.x()); }
Vector2 &LineBase::start() { return m_start; }
Vector2 &LineBase::end() { return m_end; }
const Vector2 &LineBase::start() const { return m_start; }
const Vector2 &LineBase::end() const { return m_end; }
void LineBase::move(const Vector2 &by) {
  m_start+=by;
  m_end+=by;
}
bool LineBase::isColinear(const LineBase &other) const { return isColinear(other.m_start) && isColinear(other.m_end); }
bool LineBase::isColinear(const Vector2 &point) const {
    // We check if the area of the triangle defined by 3 points is zero:
    // Google shoelace formula if you are confused by below computation:
    Vector2 A = direction();
    Vector2 B = point - m_start;
    return A.cross(B) == 0;  // TODO: test floating point accuracy?
}
