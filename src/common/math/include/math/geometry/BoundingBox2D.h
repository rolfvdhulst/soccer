//
// Created by rolf on 24-04-20.
//

#ifndef SOCCER_SRC_COMMON_MATH_GEOMETRY_BOUNDINGBOX_H_
#define SOCCER_SRC_COMMON_MATH_GEOMETRY_BOUNDINGBOX_H_

#include "Vector2.h"

class Ray;
class LineSegment;
class Line;
/**
 * This class represents an axis-aligned bounding box (AABB) in 2 dimensions
 * This is often used to do collision checking/pruning as this is quite cheap using min/max
 */
class BoundingBox2D {
   public:
    /**
     * Constructs a bounding box spanning the entire 2d plane (from (-inf,-inf) to (inf,inf))
     */
    BoundingBox2D();
    /**
     * Constructs an infinitely small bounding box around the point.
     * @param point
     */
    explicit BoundingBox2D(const Vector2 &point);
    /**
     * Constructs the smallest bounding box that encapsulates both given points
     * @param point2
     */
    BoundingBox2D(const Vector2 &point1, const Vector2 &point2);
    BoundingBox2D(double xMin, double yMin, double xMax, double yMax);

    [[nodiscard]] double xMin() const;
    [[nodiscard]] double yMin() const;
    [[nodiscard]] double xMax() const;
    [[nodiscard]] double yMax() const;

    bool contains(const Vector2& point);

    bool doesIntersect(const LineSegment& segment);

    bool doesIntersect(const Ray& ray);
    bool doesIntersectA(const Ray& ray);
    bool doesIntersectB(const Ray& ray);
    bool doesIntersect(const Line& line);
    /**
     * @param other
     * @return True if the two bounding boxes overlap somewhere
     */
    [[nodiscard]] bool doOverlap(const BoundingBox2D &other) const;
    /**
     * @param other
     * @return a bounding box 2d representing the overlap of the two bounding boxes
     */
    [[nodiscard]] std::optional<BoundingBox2D> overlap(const BoundingBox2D &other) const;
    /**
     * @param other
     * @return The smallest bounding box 2d which contains this and the other bounding box
     */
    BoundingBox2D operator+(const BoundingBox2D &other) const;
    /**
     * @param other
     * @return The smallest bounding box 2d which contains this and the point
     */
    BoundingBox2D operator+(const Vector2 &point) const;
    /**
     * @brief expands current bounding box to also contain other
     * @param other
     * @return A reference to this after being expanded.
     */
    BoundingBox2D &operator+=(const BoundingBox2D &other);
    /**
     * @param other
     * @return The smallest bounding box 2d which contains this and the point
     */
    BoundingBox2D &operator+=(const Vector2 &point);

   private:
    Vector2 min;
    Vector2 max;
};

#endif  // SOCCER_SRC_COMMON_MATH_GEOMETRY_BOUNDINGBOX_H_
