// Created by rolf on 24-04-20.
//

#ifndef SOCCER_SRC_COMMON_MATH_GEOMETRY_LINEBASE_H_
#define SOCCER_SRC_COMMON_MATH_GEOMETRY_LINEBASE_H_

#include "Vector2.h"

class Line;
class LineSegment;
class Ray;
class BoundingBox2D;
/**
 * @brief An abstract base class for line-like objects (lines, line segments and rays), which contains some shared functions.
 * Note that none of the derived classes are actually responsible for the case when the user defines a degenerate line (e.g. which has two and the same points).
 * We decided to push this responsibility to the user as it allows in a lot of places to not need to do a lot of if(isPoint()) checks,
 * which can cost a lot of performance.
 */
class LineBase {
   protected:
    Vector2 m_start;
    Vector2 m_end;

   public:
    constexpr LineBase() = default;
    constexpr LineBase(const Vector2& start, const Vector2& end) : m_start{start}, m_end{end} {
      assert(m_start!=m_end);
    };
    constexpr LineBase(const LineBase& other) = default;
    constexpr LineBase(double startX, double startY, double endX, double endY) : m_start{Vector2(startX, startY)}, m_end{Vector2(endX, endY)} {
      assert(m_start!=m_end);
    }
    /**
     * @return Start of the line-like object
     */
    Vector2& start();
    /**
     * @return End of the line-like object
     */
    Vector2& end();
    /**
     * @return Start of the line-like object
     */
    [[nodiscard]] const Vector2& start() const;
    /**
     * @return End of the line-like object
     */
    [[nodiscard]] const Vector2& end() const;
    /**
     * Moves both points on the line by the specified amount
     * @param by vector to move points with
     */
    void move(const Vector2& by);
    /**
     * @brief Checks whether this is a single point
     *
     * @return true True if  start == end
     * @return false False otherwise
     */
    [[nodiscard]] bool isPoint() const;
    /**
     * @return True if this is perfectly vertical, false otherwise.
     */
    [[nodiscard]] bool isVertical() const;
    /**
     * @return True if this is perfectly horizontal, false otherwise.
     */
    [[nodiscard]] bool isHorizontal() const;
    /**
     * @brief returns the direction in which this is pointing.
     * This is the direction that goes from start to end (e.g. implemented as (end-start))
     * @return The direction in which this is pointing.
     */
    [[nodiscard]] Vector2 direction() const;
    /**
     * @return The slope a for the equation y=ax+b which represents this
     */
    [[nodiscard]] double slope() const;
    /**
     * @return The intercept b for the equation y=ax+b which represents this
     */
    [[nodiscard]] double intercept() const;
    /**
     * @return The coefficient pair (a,b) for the equation y=ax+b which represents this
     */
    [[nodiscard]] std::pair<double, double> coefficients() const;
    /**
     * @param other line to check for with this
     * @return true if this and other are parallel.
     */
    [[nodiscard]] bool isParallel(const LineBase& other) const;
    /**
     * @param other line to check for.
     * @return true if this and other are parallel and lie exactly on the same line.
     */
    [[nodiscard]] bool isColinear(const LineBase& other) const;
    /**
     * @param other line to check for.
     * @return true if this and other are parallel and lie exactly on the same line.
     */
    [[nodiscard]] bool isColinear(const Vector2& other) const;
    /**
     * @param point
     * @return The location on this closest to given point
     */
    [[nodiscard]] virtual Vector2 project(const Vector2& point) const = 0;
    /**
     * @param point point to calculate distance to this from
     * @return distance to closest point of this
     */
    [[nodiscard]] virtual double distanceTo(const Vector2& point) const = 0;
    /**
     * @param point
     * @return True if this goes through point
     */
    [[nodiscard]] virtual bool hits(const Vector2& point) const = 0;
    /**
     * @param line
     * @return true if this intersects the line
     */
    [[nodiscard]] virtual bool doesIntersect(const Line& line) const = 0;
    /**
     * @param segment
     * @return true if this intersects given line segment
     */
    [[nodiscard]] virtual bool doesIntersect(const LineSegment& segment) const = 0;
    /**
     * @param ray
     * @return true if this intersects given ray
     */
    [[nodiscard]] virtual bool doesIntersect(const Ray& ray) const = 0;
    /**
     * @param line
     * @return an optional that contains the intersection of this and line if it exists. Otherwise, it is empty.
     */
    [[nodiscard]] virtual std::optional<Vector2> intersects(const Line& line) const = 0;
    /**
     * @param segment
     * @return an optional that contains the intersection of this and segment if it exists. Otherwise, it is empty.
     */
    [[nodiscard]] virtual std::optional<Vector2> intersects(const LineSegment& segment) const = 0;
    /**
     * @param ray
     * @return an optional that contains the intersection of this and ray if it exists. Otherwise, it is empty.
     */
    [[nodiscard]] virtual std::optional<Vector2> intersects(const Ray& ray) const = 0;

    [[nodiscard]] virtual BoundingBox2D boundingBox() const = 0;
};

#endif  // SOCCER_SRC_COMMON_MATH_GEOMETRY_LINEBASE_H_
