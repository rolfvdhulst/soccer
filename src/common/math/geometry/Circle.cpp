//
// Created by emiel on 24-02-20.
//

#include "geometry/Circle.h"
#include "geometry/Line.h"
#include "geometry/LineSegment.h"
#include "geometry/Ray.h"
#include "general/QuadraticEquation.h"
#include "geometry/Rectangle.h"
#include "geometry/BoundingBox2D.h"

void Circle::move(const Vector2 &by) {
  center+=by;
}
bool Circle::contains(const Vector2 &point) const {
  return (center-point).length2() <= radius*radius;
}
BoundingBox2D Circle::boundingBox() const {
  assert(radius >= 0); //TODO: remove again after testing
  return BoundingBox2D(center.x()-radius,center.y()-radius,center.x()+radius,center.y()+radius);
}
//TODO: benchmark different types, projection might be quite expensive compared to solving a quadratic equation nicely?
//Check distance to line segment. If atleast one of the points is outside of the circle we can conclude it must intersect the boundary.
//Note this fails if the line segment is a point which lies on the boundary (which we assume not to be the case)
bool Circle::doesIntersect(const LineSegment &segment) const {
  return (segment.project(center)-center).length2()<=radius*radius && (!contains(segment.start()) || !contains(segment.end()));
}
//Since rays are infinitely long to one side, if it approaches close enough it must intersect the circle.
bool Circle::doesIntersect(const Ray &ray) const {
  return (ray.project(center)-center).length2()<=radius*radius;
}
//https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm
std::vector<Vector2> Circle::intersects(const LineSegment &segment) const {
  Vector2 d = segment.direction();
  Vector2 f = segment.start()-center;

  double a = d.dot(d);
  double b = 2*f.dot(d);
  double c = f.dot(f)-radius*radius;
  //Note the values here are sorted ascending already
  std::optional<std::pair<double,double>> values = solveQuadraticPositiveA(a,b,c);
  if(!values){
    return {};
  }
  std::vector<Vector2> intersections;
  if(values->first>=0 && values->first<=1){
    intersections.push_back(segment.start()+d*values->first);
  }
  if(values->second >= 0 && values->second<=1 && values->second != values->first ){
    intersections.push_back(segment.start()+d*values->second);
  }
  return intersections;
}
std::vector<Vector2> Circle::intersects(const Ray &ray) const {
  Vector2 d = ray.direction();
  Vector2 f = ray.start()-center;

  double a = d.dot(d);
  double b = 2*f.dot(d);
  double c = f.dot(f)-radius*radius;
  //Note the values here are sorted ascending already
  std::optional<std::pair<double,double>> values = solveQuadraticPositiveA(a,b,c);
  if(!values){
    return {};
  }
  std::vector<Vector2> intersections;
  if(values->first>=0 ){
    intersections.push_back(ray.start()+d*values->first);
  }
  if(values->second >= 0  && values->second != values->first ){
    intersections.push_back(ray.start()+d*values->second);
  }
  return intersections;
}
bool Circle::doesIntersectOrContain(const Line &other) { return other.distanceTo(center) <= radius; }

bool Circle::doesIntersectOrContain(const LineSegment &other) { return other.distanceTo(center) <= radius; }

bool Circle::doesIntersectOrContain(const Circle &other) { return std::abs((center - other.center).length()) <= (radius + other.radius); }

bool Circle::doesIntersectOrContain(const Rectangle &other) {
    // https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection

    Vector2 rectCenter = other.center();
    Vector2 distanceToCircle = (center - rectCenter).abs();

    double rectWidth = other.width();
    double rectHeight = other.height();

    if (distanceToCircle.x() > (rectWidth / 2 + radius)) {
        return false;
    }
    if (distanceToCircle.y() > (rectHeight / 2 + radius)) {
        return false;
    }

    if (distanceToCircle.x() <= (rectWidth / 2)) {
        return true;
    }
    if (distanceToCircle.y() <= (rectHeight / 2)) {
        return true;
    }

    double cornerDistance_sq = std::pow((distanceToCircle.x() - rectWidth / 2), 2) + std::pow((distanceToCircle.y() - rectHeight / 2), 2);

    return (cornerDistance_sq <= (radius * radius));
}

bool Circle::doesIntersectOrContain2(const Rectangle &other) {
    // https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection

    if (other.contains(center)) return true;

    std::vector<LineSegment> segments = other.lines();
    for (const LineSegment &segment : segments)
        if (doesIntersectOrContain(segment)) return true;

    return false;
}

Vector2 Circle::project(const Vector2 &point) { return center + (point - center).stretchToLength(radius); }

bool Circle::operator==(const Circle &other) const { return center == other.center && radius == other.radius; }
bool Circle::operator!=(const Circle &other) const { return !(*this == other); }


std::ostream &Circle::write(std::ostream &os) const { return os << "Circle({" << center.x() << ", " << center.y() << "}, " << radius << ")"; }


std::ostream &operator<<(std::ostream &os, Circle const &circle) { return circle.write(os); }