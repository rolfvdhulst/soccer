//
// Created by rolf on 22-01-20.
//
#include "geometry/Rectangle.h"
#include "geometry/Line.h"
#include "geometry/LineSegment.h"

constexpr const unsigned int INSIDE = 0x00;
constexpr const unsigned int LEFT = 0x01;
constexpr const unsigned int RIGHT = 0x02;
constexpr const unsigned int BOTTOM = 0x04;
constexpr const unsigned int TOP = 0x08;

Rectangle::Rectangle(const Vector2 &corner, const Vector2 &oppositeCorner) {
    min = Vector2(fmin(corner.x(), oppositeCorner.x()), fmin(corner.y(), oppositeCorner.y()));
    max = Vector2(fmax(corner.x(), oppositeCorner.x()), fmax(corner.y(), oppositeCorner.y()));
}
Rectangle::Rectangle(const Vector2 &bottomLeft, double x, double y) : min{bottomLeft}, max{Vector2(bottomLeft.x() + x, bottomLeft.y() + y)} {}
unsigned int Rectangle::CohenSutherlandCode(const Vector2 &point) const {
    double x = point.x();
    double y = point.y();
    unsigned int code = INSIDE;  // initialize code as if it's inside the clip window
    if (x < minX()) {
        code |= LEFT;
    } else if (x > maxX()) {
        code |= RIGHT;
    }

    if (y < minY()) {
        code |= BOTTOM;
    } else if (y > maxY()) {
        code |= TOP;
    }
    return code;
}
double Rectangle::minX() const { return min.x(); }
double Rectangle::maxX() const { return max.x(); }
double Rectangle::minY() const { return min.y(); }
double Rectangle::maxY() const { return max.y(); }
double Rectangle::width() const { return std::abs(min.x() - max.x()); }
double Rectangle::height() const { return std::abs(min.y() - max.y()); }

std::vector<Vector2> Rectangle::corners() const {
    std::vector<Vector2> corners = {min, Vector2(minX(), maxY()), max, Vector2(maxX(), minY())};
    return corners;
}

std::vector<LineSegment> Rectangle::lines() const {
    std::vector<Vector2> points = corners();
    std::vector<LineSegment> lines = {LineSegment(points[0], points[1]), LineSegment(points[1], points[2]), LineSegment(points[2], points[3]), LineSegment(points[3], points[0])};
    return lines;
}


Vector2 Rectangle::center() const { return (min + max) * 0.5; }

// code borrowed from https://www.geeksforgeeks.org/line-clipping-set-1-cohen-sutherland-algorithm/
// and wikipedia. (I know it's way too long)
std::vector<Vector2> Rectangle::intersects(const LineSegment &line) const {
    unsigned int code0 = CohenSutherlandCode(line.start());
    unsigned int code1 = CohenSutherlandCode(line.end());
    std::vector<Vector2> intersections;
    bool accept = false;
    double x0 = line.start().x();
    double y0 = line.start().y();
    double x1 = line.end().x();
    double y1 = line.end().y();
    while (true) {
        if (!(code0 | code1)) {
            // bitwise OR is 0: both points inside window; trivially accept and exit loop
            accept = true;
            break;
        } else if (code0 & code1) {
            // bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
            // or BOTTOM), so both must be outside window; exit loop (accept is false)
            break;
        } else {
            // failed both tests, so calculate the line segment to clip
            // from an outside point to an intersection with clip edge
            double x, y;

            // At least one endpoint is outside the clip rectangle; pick it.
            unsigned int codeOut = code0 ? code0 : code1;

            // Now find the intersection point;
            // use formulas:
            //   slope = (y1 - y0) / (x1 - x0)
            //   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
            //   y = y0 + slope * (xm - x0), where xm is xmin or xmax
            // No need to worry about divide-by-zero because, in each case, the
            // outcode bit being tested guarantees the denominator is non-zero

            if (codeOut & TOP) {  // point is above the clip window
                x = x0 + (x1 - x0) * (maxY() - y0) / (y1 - y0);
                y = maxY();
            } else if (codeOut & BOTTOM) {  // point is below the clip window
                x = x0 + (x1 - x0) * (minY() - y0) / (y1 - y0);
                y = minY();
            } else if (codeOut & RIGHT) {  // point is to the right of clip window
                y = y0 + (y1 - y0) * (maxX() - x0) / (x1 - x0);
                x = maxX();
            } else if (codeOut & LEFT) {  // point is to the left of clip window
                y = y0 + (y1 - y0) * (minX() - x0) / (x1 - x0);
                x = minX();
            }
            // Now we move outside point to intersection point to clip
            // and get ready for next pass.
            if (codeOut == code0) {
                x0 = x;
                y0 = y;
                Vector2 pt = Vector2(x0, y0);
                code0 = CohenSutherlandCode(pt);
                // Save point iff it is inside the Rect
                if (code0 == INSIDE) {
                    intersections.push_back(pt);
                }
            } else {
                x1 = x;
                y1 = y;
                Vector2 pt = Vector2(x1, y1);
                code1 = CohenSutherlandCode(pt);
                // Save point iff it is inside the Rect
                if (code1 == INSIDE) {
                    intersections.push_back(pt);
                }
            }
        }
    }
    return accept ? intersections : std::vector<Vector2>();
}
bool Rectangle::doesIntersect(const LineSegment &line) const { return !intersects(line).empty(); }

std::vector<Vector2> Rectangle::intersects(const Line &line) const {
    std::vector<LineSegment> boxLines = lines();
    std::vector<Vector2> intersections;
    for (const auto &boxLine : boxLines) {
        std::optional<Vector2> point = line.intersects(boxLine);
        if (point) {
            intersections.push_back(*point);
        }
    }
    return intersections;
}
bool Rectangle::doesIntersect(const Line &line) const {
    std::vector<LineSegment> boxLines = lines();
    for (const auto &boxLine : boxLines) {
        if (line.doesIntersect(boxLine)) {
            return true;
        }
    }
    return false;
}

// include the boundary for this calculation!
bool Rectangle::contains(const Vector2 &point) const { return maxX() >= point.x() && minX() <= point.x() && maxY() >= point.y() && minY() <= point.y(); }
std::ostream &Rectangle::write(std::ostream &os) const { return os << "Rect: " << min << max; }

double Rectangle::distanceTo(const Vector2 &point) const {
    //inefficient implementation, but it does the job.
    //Could give numerical error problems if used for things like checking equality
    double minDist = std::numeric_limits<double>::infinity();
    for(const auto& line : lines()){
        double dist = line.distanceTo(point);
        if (dist < minDist){
            minDist = dist;
        }
    }
    return minDist;
}

std::ostream &operator<<(std::ostream &out, const Rectangle &rect) { return rect.write(out); }
