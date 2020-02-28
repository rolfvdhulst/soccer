//
// Created by emiel on 24-02-20.
//

#ifndef SOCCER_CIRCLE_H
#define SOCCER_CIRCLE_H

#include <vector>
#include "Vector2.h"

/**
 * @brief Represents a circle by storing the center and the radius
 * @date 24-02-2020
 * @author Emiel Steerneman
 */

class Line;
class LineSegment;
class Rectangle;

class Circle {
   public:
    Vector2 center;
    double radius;

    /**
     * @brief Constructs a unit circle with center (0, 0) and radius 1.0
     */
    constexpr Circle() : center{0, 0}, radius{1.0} {};
    /**
     * @brief Constructs a circle from a center and a radius. Ensures a positive radius
     * @param center Center of the circle
     * @param radius Radius of the circle
     */
    constexpr Circle(const Vector2& center, double radius) : center{center}, radius{std::abs(radius)} {};
    /**
     * @brief Construct a circle from another circle by copying its center and radius
     * @param other The circle from which to copy the center and radius
     */
    constexpr Circle(const Circle& other) = default;


    /**
     * @brief Checks whether the circle intersects with the given object
     * @param other The object to check intersection with
     * @return True if the circle and object intersect
     * @return False if the circle and object don't intersect
     */
    [[nodiscard]] bool doesIntersectOrContain(const Vector2& other);
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
    Vector2 project(const Vector2& point);

    /** @brief Check if two circles are equal by comparing the centers and radii */
    bool operator==(const Circle& other) const;
    bool operator!=(const Circle& other) const;

    /** @brief Moves the center of the circle by the value given by the vector */
    Circle operator+(const Vector2& other) const;
    Circle operator-(const Vector2& other) const;
    Circle operator+=(const Vector2& other);
    Circle operator-=(const Vector2& other);

    /** @brief Scales the radius of the circle by the given scale */
    Circle operator*(double scale) const;
    Circle operator/(double scale) const;
    Circle operator*=(double scale);
    Circle operator/=(double scale);

    /** @brief Writes a textual representation of this circle to the given output stream */
    std::ostream& write(std::ostream& out) const;
};

std::ostream& operator<<(std::ostream& out, const Circle& circle);


#endif  // SOCCER_CIRCLE_H
