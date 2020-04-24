//
// Created by rolf on 28-2-20.
//

#include "geometry/Line.h"
#include "geometry/LineSegment.h"
Line::Line(const LineSegment &other) : LineBase(other) {}
double Line::distanceTo(const Vector2 &point) const { return (this->project(point) - point).length(); }

/// Computes the projection of point onto the line. This is identical to picking the closest point on the line
// if we project point P onto AB we can compute as A + dot(AP,AB) / dot(AB,AB) * AB
Vector2 Line::project(const Vector2 &point) const {
    if (isPoint()) {  // Direction() == 0 makes our computation give 0/0 so all values become NaN
        return m_start;
    }
    Vector2 AB = direction();
    Vector2 AP = point - m_start;
    return m_start + AB * (AP.dot(AB) / (AB).length2());
}

bool Line::hits(const Vector2 &point) const {
    return isColinear(point);  // If a line is colinear it also actually hits the point
}

// see https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection for help. These should be thoroughly tested
std::optional<Vector2> Line::intersects(const Line &line) const {
    Vector2 A = m_start - m_end;
    Vector2 B = line.m_start - line.m_end;
    double denom = A.cross(B);
    if (denom != 0) {
        Vector2 C = m_start - line.m_start;
        double numer = -C.cross(B);
        double t = numer / denom;
        return m_start + A * t;
    }
    return std::nullopt;
}

std::optional<Vector2> Line::intersects(const LineSegment &line) const {
    Vector2 A = m_start - m_end;
    Vector2 B = line.start() - line.end();
    double denom = A.cross(B);
    if (denom != 0) {
        Vector2 C = m_start - line.start();
        double numer = C.cross(A);
        double u = numer / denom;
        if (!(u < 0 || u > 1)) {
            return line.start() - B * u;
        }
    }
    return std::nullopt;
}

bool Line::doesIntersect(const Line &line) const {
    // a line does not intersect only if it is colinear
    // We check if they are not parallel first which is a bit faster in most cases
    // as it's easier to check than colinearity and covers 99.9% of the cases
    return !(isParallel(line) && !(isColinear(line)));
}

bool Line::doesIntersect(const LineSegment &line) const {
    // TODO: optimize/test?
    if (intersects(line)) {
        return true;
    }
    return false;
}
