//
// Created by rolf on 24-04-20.
//

#include "math/geometry/BoundingBox2D.h"
#include "geometry/LineSegment.h"
#include "geometry/Ray.h"

BoundingBox2D::BoundingBox2D()
    : min{Vector2(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity())},
      max{Vector2(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity())} {}
BoundingBox2D::BoundingBox2D(const Vector2 &point) : min{point}, max{point} {}
BoundingBox2D::BoundingBox2D(const Vector2 &point1, const Vector2 &point2) {
    // determine the min and max of x and y's
    auto x = std::minmax({point1.x(), point2.x()});
    auto y = std::minmax({point1.y(), point2.y()});
    min = Vector2(x.first, y.first);
    max = Vector2(x.second, y.second);
}
BoundingBox2D::BoundingBox2D(double xMin, double yMin, double xMax, double yMax) : min{Vector2(xMin, yMin)}, max{Vector2(xMax, yMax)} {}

double BoundingBox2D::xMin() const { return min.x(); }
double BoundingBox2D::yMin() const { return min.y(); }
double BoundingBox2D::xMax() const { return max.x(); }
double BoundingBox2D::yMax() const { return max.y(); }
BoundingBox2D BoundingBox2D::operator+(const BoundingBox2D &other) const {
    BoundingBox2D box = *this;
    box += other;
    return box;
}
BoundingBox2D &BoundingBox2D::operator+=(const BoundingBox2D &other) {
    if (other.xMin() < xMin()) {
        min.x() = other.xMin();
    }
    if (other.yMin() < yMin()) {
        min.y() = other.yMin();
    }
    if (other.xMax() > xMax()) {
        max.x() = other.xMax();
    }
    if (other.yMax() > yMax()) {
        max.y() = other.yMax();
    }
    return *this;
}
BoundingBox2D BoundingBox2D::operator+(const Vector2 &point) const {
    BoundingBox2D box = *this;
    box += point;
    return box;
}
BoundingBox2D &BoundingBox2D::operator+=(const Vector2 &point) {
    double x = point.x();
    if (x < xMin()) {
        min.x() = x;
    } else if (x > xMax()) {
        max.x() = x;
    }
    double y = point.y();
    if (y < yMin()) {
        min.y() = y;
    } else if (y > yMax()) {
        max.y() = y;
    }
    return *this;
}
bool BoundingBox2D::doesOverlap(const BoundingBox2D &other) const { return (xMin() <= other.xMax() && xMax() >= other.xMin()) && (yMin() <= other.yMax() && yMax() >= other.yMin()); }
std::optional<BoundingBox2D> BoundingBox2D::overlap(const BoundingBox2D &other) const {
    if (!doesOverlap(other)) {
        return std::nullopt;
    }
    // Use fmin and fmax here as they allow the compiler to abuse the min and max functions on modern cpu's
    BoundingBox2D box{fmax(xMin(), other.xMin()), fmax(yMin(), other.yMin()), fmin(xMax(), other.xMax()), fmin(yMax(), other.yMax())};
    return box;
}
bool BoundingBox2D::contains(const Vector2 &point) {
    return point.x()>= xMin() && point.x() <= xMax() && point.y() >= yMin() && point.y() <= yMax();
}
//This function can be optimized a lot further, but for now this ain't really necessary.
//see: https://tavianator.com/fast-branchless-raybounding-box-intersections/
bool BoundingBox2D::doesIntersect(const Ray &ray) {
  Vector2 rayDir = ray.direction();
  Vector2 origin = ray.start();
  double tmin = -std::numeric_limits<double>::infinity();
  double tmax = std::numeric_limits<double>::infinity();
  if(rayDir.x() != 0.0){
    double t1 = (xMin()-origin.x()) / rayDir.x();
    double t2 = (xMax()-origin.x()) / rayDir.x();

    tmin = fmax(tmin,fmin(t1,t2));
    tmax = fmin(tmax,fmax(t1,t2));
  }else if(!(origin.x()<=xMax() && origin.x()>=xMin())){
    return false;
  }
  if(rayDir.y() != 0.0){
    double t1 = (yMin()-origin.y()) / rayDir.y();
    double t2 = (yMax()-origin.y()) / rayDir.y();

    tmin = fmax(tmin,fmin(t1,t2));
    tmax = fmin(tmax,fmax(t1,t2));
  } else if(!(origin.y()<=yMax() && origin.y()>=yMin())){
    return false;
  }
  return tmax >= tmin && tmax>=0;
}

bool BoundingBox2D::operator==(const BoundingBox2D &other) const{
  return xMin()==other.xMin() && yMin() == other.yMin() && xMax() == other.xMax() && yMax() == other.yMax();
}

