//
// Created by rolf on 26-4-20.
//

#include "geometry/Ray.h"
#include "geometry/BoundingBox2D.h"
#include "geometry/Line.h"
#include "geometry/LineSegment.h"

Ray::Ray() : LineBase(){ }
Ray::Ray(const Vector2& start, const Vector2& end) : LineBase(start,end){}
Ray::Ray(const LineSegment &segment) : LineBase(segment){ }
Ray::Ray(const Line &line) : LineBase(line) { }

double Ray::distanceTo(const Vector2 &point) const {
    return (project(point)-point).length();
}
Vector2 Ray::project(const Vector2 &point) const {
    //TODO: colinearity/ line being a point?
    Vector2 AB = direction();
    Vector2 AP = point - m_start;
    double t = AP.dot(AB) / AB.dot(AB);
    return  t <= 0 ? m_start : m_start + AB * t;
}
BoundingBox2D Ray::boundingBox() const {
    //Check which way the direction is going to infinity
    Vector2 dir = direction();
    double xMin = dir.x() < 0 ? -std::numeric_limits<double>::infinity() : m_start.x();
    double xMax = dir.x() > 0 ? std::numeric_limits<double>::infinity() : m_start.x();
    double yMin = dir.y() < 0 ? -std::numeric_limits<double>::infinity() : m_start.y();
    double yMax = dir.y() > 0 ? std::numeric_limits<double>::infinity() : m_start.y();
    return BoundingBox2D(xMin,yMin,xMax,yMax);
}
bool Ray::hits(const Vector2 &point) const {
    Vector2 A = direction();
    Vector2 B = point - m_start;
    double cross = A.cross(B);
    if (cross != 0) {
        return false; //lines are not colinear.
    }
    // check if the point is before the starting point
    double dot = A.dot(B);
    return dot >= 0;

}
std::optional<Vector2> Ray::intersects(const Line &line) const {
    Vector2 p = m_start;
    Vector2 r = direction();
    Vector2 q = line.start();
    Vector2 s = line.direction();
    double denom = r.cross(s);
    if (denom == 0) {
        // lines are parallel. we need to check if they are not on the same line:
        if ((q - p).cross(r) == 0) {
            return m_start;  // line and this line are the same, we return the start of the lineSegment (though every point on it intersects)
        } else {
            return std::nullopt;
        }
    } else {
        // Lines are not parallel
        double t = (q - p).cross(s) / denom;
        // if intersection point lies on the ray we return it
        if (t >= 0) {
            return p + r * t;
        }
    }
    return std::nullopt;
}
std::optional<Vector2> Ray::intersects(const LineSegment &segment) const {
    // These copies will get optimized away but make it easier to read w.r.t the stackoverflow link
    Vector2 p = m_start;
    Vector2 r = direction();
    Vector2 q = segment.start();
    Vector2 s = segment.direction();

    double uDenom = r.cross(s);
    double uNumer = (q - p).cross(r);
    if (uDenom == 0) {
        if (uNumer == 0) {
            // Lines are colinear;
            // if the interval between t0 and t1 intersects [0,infinity] they lines overlap on this interval
            double t0 = (q - p).dot(r) / (r.dot(r));
            double t1 = t0 + s.dot(r) / (r.dot(r));
            if(t0 < 0 && t1 < 0){
                return std::nullopt;
            }else if(t0 > 0 && t1 > 0){
                return p + r*fmin(t0,t1);
            }else{
                return p;
            }
        } else {
            return std::nullopt;  // Lines are parallel and nonintersecting
        }
    } else {
        // if we find t and u such that p+tr=q+us for t and u between 0 and 1, we have found a valid intersection.
        double u = uNumer / uDenom;
        if (u >= 0 && u <= 1) {
            double t = (q - p).cross(s) / uDenom;
            if (t >= 0) {
                return p + r * t;  // we found a intersection point!
            }
        }
    }
    return std::nullopt;
}
bool Ray::doesIntersect(const Line &line) const {
    return intersects(line) != std::nullopt; //TODO: should be easy crossproduct calculation. Just need to know side of start and direction point towards the line

}
bool Ray::doesIntersect(const LineSegment &segment) const {
    return intersects(segment) != std::nullopt;
}
bool Ray::doesIntersect(const Ray &ray) const {
  Vector2 p = m_start, q = ray.m_start, r = direction(), s = ray.direction();
  double denom = r.cross(s);
  double numer = (q - p).cross(r);
  if (denom == 0) {
    if (numer == 0) {
      // rays are colinear. it simplifies to the following statement:
      // (this is not trivial but it's the same logic as for segment-segment intersection
      double t0 = (q - p).dot(r);
      return t0>=0 || s.dot(r) >=0;
    }
  } else {
    double u = numer / denom;
    if (u>=0) {  // check if it's on the ray
      double t = (q - p).cross(s) / denom;
      return (t>=0);  // check if it's on the ray
    }
  }
  return false;
}

std::optional<Vector2> Ray::intersects(const Ray &ray) const {
  Vector2 p = m_start, q = ray.m_start, r = direction(), s = ray.direction();
  double denom = r.cross(s);
  double numer = (q - p).cross(r);
  if (denom == 0) {
    if (numer == 0) {
      // rays are colinear. it simplifies to the following statement:
      // (this is not trivial but it's the same logic as for segment-segment intersection
      double t0 = (q - p).dot(r)/r.length2();
      if (s.dot(r)>=0){ //both rays pointed in same direction
        if(t0>=0){
          return p+r*t0; // this one is 'behind' the other ray
        }
        return p; //the other ray is behind this one.
      }
      if(t0>=0){ //rays are opposite directions but starts are enough apart
        return p;
      }else{
        return std::nullopt;
      }
    }
  } else {
    double u = numer / denom;
    if (u>=0) {  // check if it's on the ray
      double t = (q - p).cross(s) / denom;
      if(t>=0){
        return p+r*t;
      }
    }
  }
  return std::nullopt;
}
