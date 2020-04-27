//
// Created by rolf on 28-04-20.
//

#ifndef SOCCER_SRC_COMMON_MATH_GENERAL_QUADRATICEQUATION_H_
#define SOCCER_SRC_COMMON_MATH_GENERAL_QUADRATICEQUATION_H_
#include <optional>
#include <vector>
/**
 * Solves a quadratic equation of the form ax^2+bx+c=0
 * @return an optional containing t0,t1, where t0<=t1. It may be that t0 == t1. If the discriminant<0 the optional is empty
 */
std::optional<std::pair<double,double>> solveQuadratic(double a, double b, double c);
std::optional<std::pair<double,double>> solveQuadraticPositiveA(double a, double b, double c);
std::vector<double> solveQuadraticVector(double a, double b, double c);
int numberQuadraticSolutions(double a, double b, double c);
#endif //SOCCER_SRC_COMMON_MATH_GENERAL_QUADRATICEQUATION_H_
