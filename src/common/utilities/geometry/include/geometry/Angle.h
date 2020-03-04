//
// Created by thijs on 28-2-19.
//

#ifndef SOCCER_ANGLE_H
#define SOCCER_ANGLE_H

//We only include this to forward CMath's M_PI constants clearly to all users of this class.
// Whatever math header you use you should use should be consistent throughout your system.
#include "Definitions.h"

class Vector2;

/**
 * @brief Angle class, mostly used for angle calculation. Default units are in radians.
 *
 */
class Angle {
   public:
    /**
     * @brief Default constructs an Angle
     * sets angle to 0.
     */
    Angle() = default;

    /**
     * @brief Copy constructor
     */
    Angle(const Angle &copy) = default;

    /**
     * @brief Construct a new Angle object
     *
     * @param angle Angle to set the initial angle to
     */
    explicit Angle(double angle);

    /**
     * @brief Construct a new Angle object from an rtt::Vector2
     *
     * @param vec vector to construct from
     */
    explicit Angle(const Vector2 &vec);

    /**
     * @brief Get the Angle as double
     *
     * @return double The angle
     */
    [[nodiscard]] double getAngle() const;

    /**
     * @brief Set the Angle object's angle
     *
     * @param other Angle to set the angle to
     */
    void setAngle(double other);

    /**
     * @brief Gets the difference in angle between this angle and other
     * is efectively the same as doing (this-other).angle
     * @param other Other angle
     * @return double angle
     */
    [[nodiscard]] double angleDiff(Angle const &other) const;

    /**
     * @brief Gets the difference between this.angle and other.
     * This is effectively the same as doing (this-other).angle
     *
     * @param other Other angle
     * @return double amount difference
     */
    [[nodiscard]] double angleDiff(double other) const;

    /**
     * @brief Converts the current Angle to a Vector2, does not consume
     *
     * @param length Length of the vector
     * @return rtt::Vector2 The new angle representation
     */
    [[nodiscard]] Vector2 toVector2(double length = 1.0) const;

    /**
     * @brief Compares two angles against each other
     *
     * @param other Rhs angle
     * @return true If this->angle - other.angle < epsilon
     * @return false if it's larger
     */
    bool operator==(const Angle &other) const;

    /**
     * @brief Compares two angles against each other
     *
     * @param scalar Other angle to compare against
     * @return true If the angle is equal to `this->angle`
     * @return false If it's not equal to this->angle
     */
    bool operator==(const double &scalar) const;

    /**
     * @brief Not equals operator
     *
     * @param other Other angle to compare against
     * @return bool !(*this == other)
     */
    bool operator!=(const Angle &other) const;

    /**
     * @brief Not equals operator
     *
     * @param scalar Other angle to compare against
     * @return bool !(*this == scalar)
     */
    bool operator!=(const double &scalar) const;


    /**
     * @brief Combines two angles
     *
     * @param other Other angle
     * @return Angle this->angle + other.angle
     */
    Angle operator+(const Angle &other) const;

    /**
     * @brief Combines two angles
     *
     * @param scalar Other angle
     * @return Angle this->angle + scalar
     */
    Angle operator+(const double &scalar) const;

    /**
     * @brief Subtracts two angles
     *
     * @param other Angle to subtract
     * @return Angle this->angle - other.angle
     */
    Angle operator-(const Angle &other) const;

    /**
     * @brief Subtracts two angles
     *
     * @param scalar Angle to subtract
     * @return Angle this->angle - other.angle
     */
    Angle operator-(const double &scalar) const;

    /**
     * @brief Sets the current angle equal to`this->angle + other.angle`
     *
     * @param other Other angle to add to `this`
     * @return Angle A copy of `*this`
     */
    Angle operator+=(const Angle &other);

    /**
     * @brief Sets the current angle equal to `this->angle + scalar`
     *
     * @param scalar Other angle to add to `this`
     * @return Angle A copy of `*this`
     */
    Angle operator+=(const double &scalar);

    /**
     * @brief Sets the current angle equal to `this->angle - other.angle`
     *
     * @param other Other angle to subtract from `this`
     * @return Angle A copy of `*this`
     */
    Angle operator-=(const Angle &other);

    /**
     * @brief Sets the current angle equal to `this->angle - scalar`
     *
     * @param scalar Other angle to subtract from `this`
     * @return Angle A copy of `*this`
     */
    Angle operator-=(const double &scalar);

    /**
     * @brief Copy assignment operator
     *
     * Explicitly constraints.
     *
     * @param scalar Scalar to set `this->angle` to
     */
    Angle &operator=(const double &scalar);

    /**
     * @brief Convert the Angle to a double.
     *
     * @return double `this->angle`
     */
    explicit operator double() const;

   private:
    /**
     * @brief Current angle stored
     */
    double angle;

    /**
     * @brief Constrains the angle to the interval [-Pi,Pi)
     *
     * @return Angle A copy of `*this`
     */
    Angle constrain();
};

#endif  // SOCCER_ANGLE_H
