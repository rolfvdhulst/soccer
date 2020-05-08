//
// Created by rolf on 28-2-20.
//

#ifndef SOCCER_LINE_H
#define SOCCER_LINE_H

#include "LineBase.h"
class Line : public LineBase {
   public:
    constexpr Line() : LineBase() {}

    /**
     * @brief Constructs a new Line object
     *
     * @param _start Start of the Line
     * @param _end End of the Line
     *
     */
    constexpr Line(const Vector2 &_start, const Vector2 &_end) : LineBase(_start, _end){};
    /**
     * @brief Constructs a new Line from a LineSegment.
     * @param other LineSegment to use for construction
     */
    explicit Line(const LineSegment &other);
    /**
     * @brief Constructs a new Line from a Ray.
     * @param other LineSegment to use for construction
     */
    explicit Line(const Ray &other);

    /**
     * @brief Gets the distance from \ref point to the line
     *
     * @param point Point to get distance to
     * @return double Distance to line
     */
    [[nodiscard]] double distanceTo(const Vector2 &point) const override;

    /**
     * @brief Checks whether a \ref point is on the line
     *
     * @param point Point to check `this` against
     * @return true If \ref point is on `this`
     * @return false If \ref point is not on `this`
     */
    [[nodiscard]] bool hits(const Vector2 &point) const override;

    /**
     * @brief Projects the point onto the line
     *
     * @param point Point to get the projection to
     * @return Vector2 Vector projection from the line to the point
     */
    [[nodiscard]] Vector2 project(const Vector2 &point) const override;

    /**
     * @brief Gets the intersection of the lines
     * See https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection for help
     *
     * @param line Line to get an intersection from
     * @return std::shared_ptr<Vector2> Vector representation of this intersection
     */
    [[nodiscard]] std::optional<Vector2> intersects(const Line &line) const override;

    /**
     * @brief Gets the intersection of the lines
     * See https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection for help
     *
     * @param line LineSegment to get an intersection from
     * @return std::shared_ptr<Vector2> Vector representation of this intersection
     */
    [[nodiscard]] std::optional<Vector2> intersects(const LineSegment &line) const override;
  /**
   * @brief Gets the intersection of the lines
   * See https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection for help
   *
   * @param line LineSegment to get an intersection from
   * @return std::shared_ptr<Vector2> Vector representation of this intersection
   */
  [[nodiscard]] std::optional<Vector2> intersects(const Ray &ray) const override;
    /**
     * @brief Checks whether \ref line intersects `this`
     *
     * @param line Line to check against
     * @return true if \ref line intersects `this`
     * @return false False if \ref line does not intersect `this`
     */
    [[nodiscard]] bool doesIntersect(const Line &line) const override;

    /**
     * @brief Checks whether \ref line intersects `this`
     *
     * @param line Line to check against
     * @return true True if \ref line intersects `this`
     * @return false False if \ref line does not intersect `this`
     */
    [[nodiscard]] bool doesIntersect(const LineSegment &line) const override;
    [[nodiscard]] bool doesIntersect(const Ray& ray) const override;

    [[nodiscard]] BoundingBox2D boundingBox() const override;
};

#endif  // SOCCER_LINE_H
