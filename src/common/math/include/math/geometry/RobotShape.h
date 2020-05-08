//
// Created by rolf on 26-04-20.
//

#ifndef SOCCER_SRC_COMMON_MATH_GEOMETRY_ROBOTSHAPE_H_
#define SOCCER_SRC_COMMON_MATH_GEOMETRY_ROBOTSHAPE_H_
#include "Circle.h"
#include "Shape.h"
#include "Angle.h"
#include "Line.h"

class RobotShape : public Shape {
 private:
  //The first three are necessary to determine the entire shape
  Circle circle;
  double centerToFront;
  Angle orientation;
  //The following is not necessary but prevents a lot of recomputation using sin() and cos() like functions which can be pretty expensive
  Line kickerLine;//We use a Line as it gives more efficient computations

 public:
  RobotShape(const Vector2 &pos, double centerToFront, double radius, Angle orientation);

  /**
   * Checks if a given point lies in front (not on) of the line which defines the dribbler.
   * This only checks if the point is on the correct side of the halfplane defined by the line passing through the dribbler
   * @param point
   * @return
   */
  [[nodiscard]] bool inFrontOfDribbler(const Vector2& point) const;

  [[nodiscard]] const Vector2& pos() const;

  [[nodiscard]] Angle angle() const;

  [[nodiscard]] Vector2 centerOfKickerPos() const;

  [[nodiscard]] LineSegment kicker() const;

  void move(const Vector2 &by) override;

  [[nodiscard]] bool contains(const Vector2 &point) const override;

  [[nodiscard]] BoundingBox2D boundingBox() const override;

  [[nodiscard]] bool doesIntersect(const LineSegment &segment) const override;

  [[nodiscard]] bool doesIntersect(const Ray &ray) const override;

  [[nodiscard]] std::vector<Vector2> intersects(const LineSegment &segment) const override;

  [[nodiscard]] std::vector<Vector2> intersects(const Ray &ray) const override;
};

#endif //SOCCER_SRC_COMMON_MATH_GEOMETRY_ROBOTSHAPE_H_
