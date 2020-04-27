//
// Created by rolf on 28-2-20.
//

#ifndef SOCCER_LINESEGMENT_H
#define SOCCER_LINESEGMENT_H

#include "LineBase.h"

/**
 * @brief LineSegment class
 *
 */
class LineSegment : public LineBase {
   public:
    /**
     * @brief Constructs a new LineSegment
     *
     */
    constexpr LineSegment() : LineBase() {}

    /**
     * @brief Constructs a LineSegment
     *
     * @param _start Start of the Line
     * @param _end End of the Line
     *
     */
    LineSegment(const Vector2 &_start, const Vector2 &_end) : LineBase(_start, _end){};
    /**
     * @brief Construct a LineSegment from a line.
     * @param line Line to construct LineSegment from
     */
    explicit LineSegment(const Line &line);

    explicit LineSegment(const Ray &ray);

    /**
     * @return The middle point of the end and start point (e.g. the exact middle)
     */
    [[nodiscard]] Vector2 center() const;
    /**
     * @brief Gets the length of the vector representation of this line
     * Literally:
     *      (end - start).length()
     * @return double Gets the length of the line
     */
    [[nodiscard]] double length() const;

    /**
     * @brief Gets the length of the vector representation of this line
     *
     * @return double Length of this Line
     */
    [[nodiscard]] double length2() const;
    /**
     * @brief swaps the start and end of this LineSegment so that the direction is turned by 180 degrees
     */
    void reverse();
    /**
     * @return The same LineSegment but with end and start reversed
     */
    [[nodiscard]] LineSegment reversed() const;

    /**
     * @brief Gets the distance from the line to a point
     *
     * @param point Point to get distance to
     * @return double distance between line and point
     */
    [[nodiscard]] double distanceTo(const Vector2 &point) const override;
    /**
     * @brief Gets the shortest distance between any two points on both line segments
     * @param point Point to get distance to
     * @return distance between this line and the passed line.
     */
    [[nodiscard]] double distanceTo(const LineSegment &line) const;
    /**
     * @brief Checks whether a point is on the line
     *
     * @param point Point to check
     * @return true True if the point is on this line
     * @return false False if the point is not on this line
     */
    [[nodiscard]] bool hits(const Vector2 &point) const override;

    /**
     * @brief Gets the projection of \ref point to `this`
     *
     * @param point Point to project
     * @return Vector2 Vector representation of this projectoin
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
  /**
* @brief Checks whether \ref line intersects `this`
*
* @param line Line to check against
* @return true True if \ref line intersects `this`
* @return false False if \ref line does not intersect `this`
*/
  [[nodiscard]] bool doesIntersect(const Ray &ray) const override;

    /**
     * @brief Same as normal intersect, but always returns false if the lines are parallel
     * intersection points of non-parallel lines are called non-simple (hence the name)
     *
     * @param line Line to check against
     * @return true True if \ref line intersects `this`
     * @return false False if \ref `line` does not intersect `this`
     */
    [[nodiscard]] bool nonSimpleDoesIntersect(const LineSegment &line) const;

    /**
     * @brief Gets a vector representation of an intersection
     *
     * same as normal intersect, but always returns false if the lines are parallel
     * intersection points of non-parallel lines are called non-simple (hence the name)
     *
     * @param line Line to check against
     * @return std::shared_ptr<Vector2> Returns a shared_ptr to a Vector2 that represents the intersection
     */
    [[nodiscard]] std::optional<Vector2> nonSimpleIntersects(const LineSegment &line) const;

    [[nodiscard]] BoundingBox2D boundingBox() const override;
};

#endif  // SOCCER_LINESEGMENT_H
