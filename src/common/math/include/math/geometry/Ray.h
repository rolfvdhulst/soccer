//
// Created by rolf on 26-4-20.
//

#ifndef SOCCER_RAY_H
#define SOCCER_RAY_H

#include "LineBase.h"
class Ray : public LineBase{
    public:
        Ray();
        Ray(const Vector2& start,const Vector2& end);
        explicit Ray(const LineSegment& segment);
        explicit Ray(const Line& line);
        /**
         * @brief Gets the distance from \ref point to the ray
         *
         * @param point Point to get distance to
         * @return double Distance to the ray
         */
        [[nodiscard]] double distanceTo(const Vector2 &point) const override;

        /**
         * @brief Checks whether a \ref point is on the ray
         *
         * @param point Point to check `this` against
         * @return true If \ref point is on `this`
         * @return false If \ref point is not on `this`
         */
        [[nodiscard]] bool hits(const Vector2 &point) const override;

        /**
         * @brief Projects the point onto the ray
         *
         * @param point Point to get the projection to
         * @return Vector2 Vector projection from the point to the ray
         */
        [[nodiscard]] Vector2 project(const Vector2 &point) const override;

        /**
         * @brief Gets the intersection of the lines
         * See https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection for help
         *
         * @param line Line to get an intersection from
         * @return std::optional<Vector2> Vector representation of this intersection
         */
        [[nodiscard]] std::optional<Vector2> intersects(const Line &line) const override;

        /**
         * @brief Gets the intersection of the lines
         * See https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection for help
         *
         * @param line LineSegment to get an intersection from
         * @return std::shared_ptr<Vector2> Vector representation of this intersection
         */
        [[nodiscard]] std::optional<Vector2> intersects(const LineSegment &segment) const override;

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
        [[nodiscard]] bool doesIntersect(const LineSegment &segment) const override;

        [[nodiscard]] BoundingBox2D boundingBox() const override;
};

#endif //SOCCER_RAY_H
