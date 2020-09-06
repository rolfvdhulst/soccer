//
// Created by emiel on 24-02-20.
//

#ifndef SOCCER_CIRCLE_H
#define SOCCER_CIRCLE_H

#include "Shape.h"
#include "Vector2.h"

/**
 * @brief Represents a circle by storing the center and the radius
 * @date 24-02-2020
 * @author Emiel Steerneman
 */

class Line;
class LineSegment;
class Rectangle;

class Circle : public Shape{
 private:
  Vector2 m_center;
  double m_radius;
 public:
  /**
   * @brief Constructs a unit circle with center (0, 0) and radius 1.0
   */
  constexpr Circle() : m_center{0, 0}, m_radius{1.0} {};
  /**
   * @brief Constructs a circle from a center and a radius. Ensures a positive radius
   * @param center Center of the circle
   * @param radius Radius of the circle
   */
  constexpr Circle(const Vector2& center, double radius) : m_center{center}, m_radius{std::abs(radius)} { };
  /**
   * @brief Construct a circle from another circle by copying its center and radius
   * @param other The circle from which to copy the center and radius
   */
  constexpr Circle(const Circle& other) = default;
  /**
   * Moves the circle to another place
   * @param Vector to move by
   */
  void move(const Vector2& by) override;
  /**
   * Checks if a given point lies inside the circle
   * @param point
   * @return
   */
  [[nodiscard]] bool contains(const Vector2& point) const override;
  /**
   * Returns this circle's bounding box.
   * @return
   */
  [[nodiscard]] BoundingBox2D boundingBox() const override;
  /**
   * @brief Checks if the given segment intersects the circle boundary.
   * Warning! This checks if the line segment intersects the circle's boundary, and not the entire circle.
   * To check if the segment is either within the circle or on it's boundary, use overlaps()!
   * @param segment
   * @return true if segment intersects the boundary
   */
  [[nodiscard]] bool doesIntersect(const LineSegment &segment) const override;
  /**
   * Checks if the ray intersects the circle
   * @param ray
   * @return
   */
  [[nodiscard]] bool doesIntersect(const Ray& ray) const override;
  /**
   * Returns intersection points of the line segment. The intersections are ordered in distance from the start of the segment.
   * @param segment
   * @return
   */
  [[nodiscard]] std::vector<Vector2> intersects(const LineSegment &segment) const override;

  [[nodiscard]] std::vector<double> intersectionParametrized(const LineSegment &segment) const;

  /**
   * Returns intersection points of the line segment. The intersections are ordered in distance from the start of the ray.
   * @param segment
   * @return
   */
  [[nodiscard]] std::vector<Vector2> intersects(const Ray &ray) const override;
  /**
   * @brief Checks whether the circle intersects with the given object
   * @param other The object to check intersection with
   * @return True if the circle and object intersect
   * @return False if the circle and object don't intersect
   */
  [[nodiscard]] bool doesIntersectOrContain(const Line& other);
  [[nodiscard]] bool doesIntersectOrContain(const LineSegment& other);
  [[nodiscard]] bool doesIntersectOrContain(const Circle& other);
  [[nodiscard]] bool doesIntersectOrContain(const Rectangle& other);   // This method is faster
  [[nodiscard]] bool doesIntersectOrContain2(const Rectangle& other);  // This method is easier to understand and handles rotated rectangles

  /**
   * @brief Projects the given point onto this circle
   * @param other The point which to project onto this circle
   * @return The location where the point is projected onto the circle
   */
  Vector2 project(const Vector2& point) const;

  /** @brief Check if two circles are equal by comparing the centers and radii */
  bool operator==(const Circle& other) const;
  bool operator!=(const Circle& other) const;

  const Vector2& center() const;
  const double& radius() const;

  /** @brief Writes a textual representation of this circle to the given output stream */
  std::ostream& write(std::ostream& out) const;
};

std::ostream& operator<<(std::ostream& out, const Circle& circle);

#endif  // SOCCER_CIRCLE_H
