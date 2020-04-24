//
// Created by rolf on 24-04-20.
//

#ifndef SOCCER_SRC_COMMON_MATH_GEOMETRY_SHAPE_H_
#define SOCCER_SRC_COMMON_MATH_GEOMETRY_SHAPE_H_

#include <vector>
/**
 * @brief Abstract base class for 2 dimensional shapes. Any 2 dimensional shape must atleast implement the functions specified here
 */
class Vector2;
class LineSegment;
class Ray;
class Shape {
   public:
    [[nodiscard]] virtual bool contains(const Vector2& point) const = 0;
    [[nodiscard]] virtual Rectangle boundingBox() const = 0;
    [[nodiscard]] virtual bool doesIntersect(const LineSegment& segment) const = 0;
    [[nodiscard]] virtual bool doesIntersect(const Ray& ray) const = 0;
    [[nodiscard]] virtual std::vector<Vector2> intersects(const LineSegment& segment) = 0;
    [[nodiscard]] virtual std::vector<Vector2> intersects(const Ray& ray) = 0;
};

#endif  // SOCCER_SRC_COMMON_MATH_GEOMETRY_SHAPE_H_
