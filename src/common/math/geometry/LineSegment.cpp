//
// Created by rolf on 28-2-20.
//

#include "geometry/LineSegment.h"
#include "geometry/Line.h"

LineSegment::LineSegment(const Line &line) {
    start = line.start;
    end = line.end;
}
double LineSegment::length() const { return (end - start).length(); }

double LineSegment::length2() const { return (end - start).length2(); }

double LineSegment::slope() const { return (end.y() - start.y()) / (end.x() - start.x()); }

bool LineSegment::isVertical() const { return (end.x() == start.x()) && (end.y() != start.y()); }

Vector2 LineSegment::direction() const { return Vector2(end - start); }

double LineSegment::intercept() const { return start.y() - this->slope() * start.x(); }

std::pair<double, double> LineSegment::coefficients() const { return {slope(), intercept()}; }

bool LineSegment::isPoint() const { return start == end; }

bool LineSegment::isParallel(const LineSegment &line) const {
    // check if line is vertical, otherwise check the slope
    if (this->isVertical() || line.isVertical()) {
        return this->isVertical() && line.isVertical();
    }
    return this->slope() == line.slope();
}
bool LineSegment::isParallel(const Line &line) const {
    // check if line is vertical, otherwise check the slope
    if (this->isVertical() || line.isVertical()) {
        return this->isVertical() && line.isVertical();
    }
    return this->slope() == line.slope();
}
bool LineSegment::doesIntersect(const Line &line) const {
    if (intersects(line)) {
        return true;
    }
    return false;
}

// this is the algorithm from
// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
// takes overlaps into account in contrast to the intersect() function
bool LineSegment::doesIntersect(const LineSegment &line) const {
    Vector2 p = start, q = line.start, r = direction(), s = line.direction();
    double denom = r.cross(s);
    double numer = (q - p).cross(r);
    if (denom == 0) {
        if (numer == 0) {
            // lines are colinear
            double t0 = (q - p).dot(r) / r.length2();
            double t1 = t0 + s.dot(r) / r.length2();
            if (t0 < 0) {
                return t1 >= 0;
            } else if (t0 > 1) {
                return t1 <= 1;
            }
            return true;
        }
    } else {
        double u = numer / denom;
        if (!(u < 0 || u > 1)) {  // check if it's on the segment
            double t = (q - p).cross(s) / denom;
            return (!(t < 0 || t > 1));  // check if it's on the segment
        }
    }
    return false;
}

bool LineSegment::nonSimpleDoesIntersect(const LineSegment &line) const {
    Vector2 p = start, q = line.start, r = direction(), s = line.direction();
    double denom = r.cross(s);
    double numer = (q - p).cross(r);
    if (denom != 0) {
        double u = numer / denom;
        if (!(u <= 0 || u >= 1)) {
            double t = (q - p).cross(s) / denom;
            if (!(t <= 0 || t >= 1)) {
                return true;
            }
        }
    }
    return false;
}

//see intersects below. This is basically the same but slightly modified to accomodate the fact that line is infinite
std::optional<Vector2> LineSegment::intersects(const Line &line) const {
    Vector2 p = start;
    Vector2 r = end - start;
    Vector2 q = line.start;
    Vector2 s = line.end-line.start;
    double denom = r.cross(s);
    if (denom == 0) {
        //lines are parallel. we need to check if they are not on the same line:
        if((q-p).cross(r) == 0){
            return start;//line and this line are the same, we return the start of the lineSegment (though every point on it intersects)
        }else{
            return std::nullopt;
        }
    }else{
        //Lines are not parallel
        double t = (q-p).cross(s) /denom;
        //if intersection point lies on segment we return it
        if(t>=0 && t<=1){
            return p+r*t;
        }
    }
    return std::nullopt;
}

// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
std::optional<Vector2> LineSegment::intersects(const LineSegment &line) const {
    //These copies will get optimized away but make it easier to read w.r.t the stackoverflow link
    Vector2 p = start;
    Vector2 r = end - start;
    Vector2 q = line.start;
    Vector2 s = line.end-line.start;

    double uDenom = r.cross(s);
    double uNumer = (q-p).cross(r);
    if(uDenom == 0){
        if(uNumer == 0){
            //Lines are colinear;
            //if the interval between t0 and t1 intersects [0,1] they lines overlap on this interval
            double t0 = (q-p).dot(r) /(r.dot(r));
            double t1 = t0 + s.dot(r) /(r.dot(r));
            if(t0<0){
                if(t1>=0){
                    //interval overlaps, we pick closest point which is from the start to the end of the line (p+0*r);
                    return p;
                }
            } else if(t0>1){
                if(t1<=1){
                    return p+r;// Similar but now we have the end of the line
                }
            } else{
                //t0 is between 0 and one so we just return that point
                return p+r*t0;
            }
            return std::nullopt; //there was no intersection with the interval [0,1] so the lineas are colinear but have no overlap
        } else{
            return std::nullopt;//Lines are parallel and nonintersecting
        }
    }else{
        //if we find t and u such that p+tr=q+us for t and u between 0 and 1, we have found a valid intersection.
        double u = uNumer/ uDenom;
        if(u>=0&&u<=1){
            double t = (q-p).cross(s)/uDenom;
            if(t>=0 && t<=1){
                return p+r*t;//we found a intersection point!
            }
        }
    }
    return std::nullopt;
}

double LineSegment::distanceToLine(const Vector2 &point) const { return (this->project(point) - point).length(); }

// same principle but now we do not necessarily have an orthogonal vector but just pick the closest point on the segment
Vector2 LineSegment::project(const Vector2 &point) const {
    Vector2 AB = end-start;
    Vector2 AP = point - start;
    double t = AP.dot(AB) / length2();
    if (t < 0) {
        return Vector2(start);
    } else if (t > 1) {
        return Vector2(end);
    }
    return Vector2(start + AB * t);
}

bool LineSegment::isOnLine(const Vector2 &point) const {
    Vector2 A = end - start;
    Vector2 B = point - start;
    double cross = A.cross(B);
    if (cross != 0) {
        return false;
    }
    // check if the point is in between the two points
    double dot = A.dot(B);
    if (dot < 0) {
        return false;
    }
    if (dot > A.dot(A)) {
        return false;
    }
    return true;
}

std::optional<Vector2> LineSegment::nonSimpleIntersects(const LineSegment &line) const {
    Vector2 A = start - end;
    Vector2 B = line.start - line.end;
    Vector2 C = start - line.start;
    double denom = A.cross(B);
    if (denom != 0) {
        double t = C.cross(B) / denom;
        double u = -A.cross(C) / denom;
        if (!(t <= 0 || t >= 1) && !(u <= 0 || u >= 1)) {
            return start - A * t;
        }
    }
    return std::nullopt;
}

// http://geomalgorithms.com/a07-_distance.html#dist3D_Segment_to_Segment
// for implementation hints. This is complicated unfortunately. There are methods that are a lot simpler but they are also much slower.
//TODO: write better unit tests
double LineSegment::distanceToLine(const LineSegment &line) const {
    Vector2 u = end - start;
    Vector2 v = line.end - line.start;
    Vector2 w = start - line.start;
    double uu = u.dot(u);
    double uv = u.dot(v);
    double vv = v.dot(v);
    double uw = u.dot(w);
    double vw = v.dot(w);
    double D = uu * vv - uv * uv;  // Always >=0 by definition

    double sN, sD = D;  // sc = sN / sD, default sD = D >= 0
    double tN, tD = D;  // tc = tN / tD, default tD = D >= 0

    // compute the line parameters of the two closest points
    if (D == 0) {  // the lines are parallel
        sN = 0.0;  // force using point start on this segment
        sD = 1.0;  // to prevent possible division by 0.0 later
        tN = vw;
        tD = vv;
    } else {  // get the closest points on the infinite lines
        sN = (uv * vw - vv * uw);
        tN = (uu * vw - uv * uw);
        if (sN < 0.0) {  // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = vw;
            tD = vv;
        } else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = vw + uv;
            tD = vv;
        }
    }

    if (tN < 0.0) {  // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-uw < 0.0)
            sN = 0.0;
        else if (-uw > uu)
            sN = sD;
        else {
            sN = -uw;
            sD = uu;
        }
    } else if (tN > tD) {  // tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-uw + uv) < 0.0)
            sN = 0;
        else if ((-uw + uv) > uu)
            sN = sD;
        else {
            sN = (-uw + uv);
            sD = uu;
        }
    }
    double sc = sN == 0.0 ? 0.0 : sN / sD;
    double tc = tN == 0.0 ? 0.0 : tN / tD;
    Vector2 diff = w + (u * sc) - (v * tc);
    return diff.length();
}

void LineSegment::reverse() { std::swap(start, end); }

LineSegment LineSegment::reversed() const { return {end, start}; }

Vector2 LineSegment::center() const { return (start + end) * 0.5; }
