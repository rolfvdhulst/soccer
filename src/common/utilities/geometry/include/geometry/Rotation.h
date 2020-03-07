//
// Created by rolf on 28-2-20.
//

#ifndef SOCCER_ROTATION_H
#define SOCCER_ROTATION_H
//We only include this to forward CMath's M_PI constants clearly to all users of this class.
// Whatever math header you use you should use should be consistent throughout your system.
#include "Definitions.h"

//Same as angle class but does not maintain limits
class Rotation {
    private:
        /**
         * @brief Current rotation stored
         */
        double rotation;
    public:
        Rotation() : rotation{0}{};

        explicit Rotation(double rotation);

        [[nodiscard]] double getRotation() const;
        void setRotation(double rotation);
        /**
         * @brief Compares two Rotations against each other
         * @param  other Rhs Rotation
         * @return true If this->rotation - other.rotation< epsilon
         * @return false if it's larger
         */
        bool operator==(const Rotation &other) const;

        /**
         * @brief Compares two Rotations against each other
         *
         * @param scalar Other Rotation to compare against
         * @return true If the Rotation is equal to `this->rotation`
         * @return false If it's not equal to this->rotation
         */
        bool operator==(const double &scalar) const;

        /**
         * @brief Not equals operator
         *
         * @param other Other Rotation to compare against
         * @return bool !(*this == other)
         */
        bool operator!=(const Rotation &other) const;

        /**
         * @brief Not equals operator
         *
         * @param scalar Other Rotation to compare against
         * @return bool !(*this == scalar)
         */
        bool operator!=(const double &scalar) const;

        /**
         * @brief Combines two Rotations
         *
         * @param other Other Rotation
         * @return Rotation this->rotation + other.rotation
         */
        Rotation operator+(const Rotation &other) const;

        /**
         * @brief Combines two Rotations
         *
         * @param scalar Other Rotation
         * @return Rotation this->rotation + scalar
         */
        Rotation operator+(const double &scalar) const;

        /**
         * @brief Subtracts two Rotations
         *
         * @param other Rotation to subtract
         * @return Rotation this->rotation - other.rotation
         */
        Rotation operator-(const Rotation &other) const;

        /**
         * @brief Subtracts two Rotations
         *
         * @param scalar Rotation to subtract
         * @return Rotation this->rotation - other.rotation
         */
        Rotation operator-(const double &scalar) const;

        /**
         * @brief Sets the current Rotation equal to`this->rotation + other.rotation`
         *
         * @param other Other Rotation to add to `this`
         * @return Rotation A copy of `*this`
         */
        Rotation operator+=(const Rotation &other);

        /**
         * @brief Sets the current Rotation equal to `this->rotation + scalar`
         *
         * @param scalar Other Rotation to add to `this`
         * @return Rotation A copy of `*this`
         */
        Rotation operator+=(const double &scalar);

        /**
         * @brief Sets the current Rotation equal to `this->rotation - other.rotation`
         *
         * @param other Other Rotation to subtract from `this`
         * @return Rotation A copy of `*this`
         */
        Rotation operator-=(const Rotation &other);

        /**
         * @brief Sets the current Rotation equal to `this->rotation - scalar`
         *
         * @param scalar Other Rotation to subtract from `this`
         * @return Rotation A copy of `*this`
         */
        Rotation operator-=(const double &scalar);

        /**
         * @brief Convert the Rotation to a double.
         *
         * @return double `this->rotation`
         */
        explicit operator double() const;
};

#endif  // SOCCER_ROTATION_H
