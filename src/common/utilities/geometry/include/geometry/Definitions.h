//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_DEFINITIONS_H
#define SOCCER_DEFINITIONS_H

/**
 * @date 22 jan. 2020
 * @author Rolf
 * This header contains compile-time definitions which we use all across our system, which are implicitly or explicitly
 * used.
 */


/**
 * This header has all of the most useful functions for mathematics provided by STL in it (e.g. absolute, min, max cos, sin) etc.
 * We define it here because it tends to be used in many places implicitly later and we want consistency.
 */
#include <cmath>

/**
 * A comparison precision limit in our system.
 * If instances from Vector2 or Angle match to this degree we consider them 'equal' in length as a precaution against
 * floating point errors.
 */
constexpr static double VECTOR_PRECISION=0.00001;

#endif //SOCCER_DEFINITIONS_H
