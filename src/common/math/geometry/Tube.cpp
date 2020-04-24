//
// Created by rolf on 24-02-20.
//

#include "geometry/Tube.h"
#include "geometry/Circle.h"

bool Tube::contains(const Vector2& point) const {
    return lineSegment.distanceTo(point) <= radius;  // If the point is within radius of the line the tube contains the point.
}
bool Tube::doesIntersectOrContain(const LineSegment& line) const {
    return lineSegment.distanceTo(line) <= radius;  // If the distance between the two lines is smaller than radius the line intersects the tube
}
bool Tube::isCircle() const { return lineSegment.isPoint(); }
Tube::Tube(const LineSegment& line, double radius) : lineSegment{line}, radius{radius} {}
Tube::Tube(const Vector2& start, const Vector2& end, double radius) : lineSegment{LineSegment(start, end)}, radius{radius} {}
Tube::Tube() : lineSegment{}, radius{1.0} {  // lines default construct to (0,0) to (0,0)
}
bool Tube::doesIntersectOrContain(const Circle& circle) const { return lineSegment.distanceTo(circle.center) <= (radius + circle.radius); }
