//
// Created by rolf on 28-2-20.
//

#include "geometry/Line.h"
#include "geometry/LineSegment.h"
#include "geometry/BoundingBox2D.h"
#include "geometry/Ray.h"

Line::Line(const LineSegment &other) : LineBase(other) {}
double Line::distanceTo(const Vector2 &point) const { return (this->project(point) - point).length(); }

/// Computes the projection of point onto the line. This is identical to picking the closest point on the line
// if we project point P onto AB we can compute as A + dot(AP,AB) / dot(AB,AB) * AB
Vector2 Line::project(const Vector2 &point) const {
  //Note this function fails if the line is a point.
    Vector2 AB = direction();
    Vector2 AP = point - m_start;
    return m_start + AB * (AP.dot(AB) / (AB).length2());
}

bool Line::hits(const Vector2 &point) const {
    return isColinear(point);  // If a line is colinear it also actually hits the point
}

// see https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect for help. These should be thoroughly tested
std::optional<Vector2> Line::intersects(const Line &line) const {
  Vector2 q = m_start;
  Vector2 s = direction();
  Vector2 p= line.m_start;
  Vector2 r = line.direction();
  double numer = (q-p).cross(r);
  double denom = r.cross(s);
  if (denom != 0) {
    double u = numer / denom;
    return r = q +s*u;
  }else if(numer !=0){
    return std::nullopt;//lines are parallel and non-intersecting
  }
  //lines must be colinear. For this case they are exactly the same line, so we just return the starting point of the current line;
  return q;
}

std::optional<Vector2> Line::intersects(const LineSegment &line) const {
  Vector2 p = m_start;
  Vector2 r = direction();
  Vector2 q = line.start();
  Vector2 s = line.direction();
  double numer = (q-p).cross(r);
  double denom = r.cross(s);
  if (denom != 0) {
    double u = numer / denom;
    if(u>=0 && u<=1){
      return r = q +s*u;
    }else{
      return std::nullopt;
    }
  }else if(numer !=0){
    return std::nullopt;//lines are parallel
  }
  //The lines are colinear. In this case, they always intersect. We find t0 and t1:
  double t0 = (q-p).dot(r)/r.dot(r);
  double t1 = t0+s.dot(r)/r.dot(r);
  //we find the intersection of t0,t1 which is closest to the starting point.
  if((t0>=0 && t1<0)||(t1>=0 && t0<0)){
    return p; //The segment overlaps the line starting point
  } else if(t0>=0){
    return p+r*fmin(t0,t1);
  }
  return p+r*fmax(t0,t1);
}

bool Line::doesIntersect(const Line &line) const {
    // a line does not intersect only if it is parallel
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
BoundingBox2D Line::boundingBox() const {
  if(isVertical()){
    return BoundingBox2D(m_start.x(),-std::numeric_limits<double>::infinity(),m_start.x(),std::numeric_limits<double>::infinity());
  }else if(isHorizontal()){
    return BoundingBox2D(-std::numeric_limits<double>::infinity(),m_start.y(),std::numeric_limits<double>::infinity(),m_start.y());
  }
  return BoundingBox2D();//returns a bounding box which is infinite in both directions
}
std::optional<Vector2> Line::intersects(const Ray &ray) const {
  Vector2 p = m_start;
  Vector2 r = direction();
  Vector2 q = ray.start();
  Vector2 s = ray.direction();
  double numer = (q-p).cross(r);
  double denom = r.cross(s);
  if (denom != 0) {
    double u = numer / denom;
    if(u>=0){
      return r = q +s*u;
    }else{
      return std::nullopt;
    }
  }else if(numer !=0){
    return std::nullopt;//lines are parallel
  }
  //The lines are colinear. In this case, they always intersect. We find t0 and t1:
  double t0 = (q-p).dot(r)/r.dot(r);
  double t1 = s.dot(r);
  //we find the intersection of t0,t1 which is closest to the starting point.
  if((t0>=0 && t1<0)||(t1>=0 && t0<0)){
    return p; //The segment overlaps the line starting point
  }
  return q;
}
bool Line::doesIntersect(const Ray &ray) const {
  return intersects(ray) != std::nullopt;
}


