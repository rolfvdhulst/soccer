//
// Created by rolf on 15-03-20.
//

#ifndef SOCCER_FLIP_H
#define SOCCER_FLIP_H

#include "LineSegment.h"
#include "Rectangle.h"
#include "Vector2.h"

void flipAndSwap(double &a, double &b) {
    a *= -1;
    b *= -1;
    std::swap(a, b);
}
void flipAndSwap(Vector2 &a, Vector2 &b) {
    a *= -1;
    b *= -1;
    std::swap(a, b);
}
void flipAndSwap(LineSegment &a, LineSegment &b) {
    flipAndSwap(a.start, b.start);
    flipAndSwap(a.end, b.end);
}
void flipAndSwap(Rectangle &a, Rectangle &b) {
    flipAndSwap(a.min, b.max);
    flipAndSwap(a.max, b.min);
}
#endif  // SOCCER_FLIP_H
