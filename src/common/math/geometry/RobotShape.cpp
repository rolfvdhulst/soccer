//
// Created by rolf on 26-04-20.
//

#include "math/geometry/RobotShape.h"
#include "geometry/BoundingBox2D.h"
#include "geometry/LineSegment.h"
#include "geometry/Ray.h"

RobotShape::RobotShape(const Vector2 &pos, double centerToFront, double radius, Angle orientation)
:circle{Circle(pos,radius)}, centerToFront{centerToFront}, orientation{orientation}{
  assert(centerToFront<=radius);
  //Slightly more complicated, but we do so to minimize the amount of times we need to call cos() and sin()
  Vector2 middleToCenter = Vector2(orientation).stretchToLength(centerToFront);
  Vector2 dribblerCenter = pos + middleToCenter;
  double halfFrontWidth = sqrt(radius*radius-centerToFront*centerToFront); //Pythagoras
  Vector2 diff = Vector2(-middleToCenter.y(),middleToCenter.x()).stretchToLength(halfFrontWidth);
  kickerLine = Line(dribblerCenter-diff,dribblerCenter+diff); //Lower corner, upper corner (when robot is facing right (e.g. angle = 0))
}
bool RobotShape::inFrontOfDribbler(const Vector2 &point) const {
  //We use the cross product to determine which side of the plane the point is on.
  //KickerLine is always oriented such that this definition is consistent with the ball being ahead of the robot.
  //Note we don't do >=0 because we want to exclude the case where the point is exactly on the line.
  return kickerLine.direction().cross(kickerLine.start()-point)>0;
}
void RobotShape::move(const Vector2 &by) {
  circle.move(by);
  kickerLine.move(by);
}
bool RobotShape::contains(const Vector2 &point) const {
  return circle.contains(point) && !inFrontOfDribbler(point);
}
//We return the circle's bounding box. Strictly speaking the bounding box can be smaller depending on the robots orientation
//But frankly, this probably costs more time to compute than it saves in computation later. This is just easier to work with.
BoundingBox2D RobotShape::boundingBox() const {
  return circle.boundingBox();
}
bool RobotShape::doesIntersect(const LineSegment &segment) const {
  std::vector<Vector2> intersects = circle.intersects(segment);
  if(intersects.empty()){
    //No intersections with circle, which means segment is either completely outside or completely inside the circle
    // We need to check for the case where the segment is completely inside and hits the dribbler
    return circle.contains(segment.start()) && circle.contains(segment.end()) && kickerLine.doesIntersect(segment);
  } else if (intersects.size() == 1){
    //Either the segment touches the circle, or (more likely) it starts or ends inside of it.
    if(inFrontOfDribbler(intersects[0])){
      //the segment might still intersect with the kicker, if not, there is no intersection
      return LineSegment(kickerLine).doesIntersect(segment);
    }
    //The intersection point is on the hull
    return true;
  } else if (intersects.size() == 2){
    bool firstInFront = inFrontOfDribbler(intersects[0]);
    bool secondInFront = inFrontOfDribbler(intersects[1]);
    return !(firstInFront&&secondInFront);
  }
  return false;
}
bool RobotShape::doesIntersect(const Ray &ray) const {
  std::vector<Vector2> intersects = circle.intersects(ray);
  if(intersects.empty()){
    return false;
  } else if (intersects.size() == 1){
    //Either the ray touches the circle, or (more likely) it started  inside of it.
    if(inFrontOfDribbler(intersects[0])){
      //The ray might still have hit the kicker by starting inside of the robot
      return LineSegment(kickerLine).doesIntersect(ray);
    }
    return true;//there is at least one intersection point on the hull
  } else if (intersects.size() == 2){
    bool firstInFront = inFrontOfDribbler(intersects[0]);
    bool secondInFront = inFrontOfDribbler(intersects[1]);
    return !(firstInFront&&secondInFront);
  }
  return false;//This should never be hit as circle intersection always hits atleast two.
}
std::vector<Vector2> RobotShape::intersects(const LineSegment &segment) const {
  std::vector<Vector2> intersects = circle.intersects(segment);
  if(intersects.empty()){
    //No intersections with circle, which means segment is either completely outside or completely inside the circle
    // We need to check for the case where the segment is completely inside and hits the dribbler
    //TODO: might be refactorable to not use contains but  just use LineSegment(kickerLine).intersects(segment) (check performance)
    if(circle.contains(segment.start()) && circle.contains(segment.end())){ //one of these checks should technically be redundant. But not optimizing for now.
      //then we simply compute intersections of the kicker line with the line segment
      auto intersect = kickerLine.intersects(segment);
      if(intersect){
        return {*intersect};
      }
    }
    //The segment is completely outside the circle.
    return {};
  } else if (intersects.size() == 1){
    auto intersect = segment.intersects(LineSegment(kickerLine)); // we need this info in all branches
    //Either the segment touches the circle, or (more likely) it starts or ends inside of it.
    if(inFrontOfDribbler(intersects[0])){
      //The segment might still intersect with the kicker
      if(intersect){
        return {*intersect};
      }
      return {};
    }
    //If the intersection point is on the hull there is no problem. However, there might be another intersection at the kicker.
    if(intersect){
      //Check which point was the first collision
      if((segment.start()-*intersect).length2()<(segment.start()-intersects[0]).length2()){
        return {*intersect,intersects[0]};
      }
      intersects.push_back(*intersect);
    }
    return intersects;
  } else if (intersects.size() == 2){
    bool firstInFront = inFrontOfDribbler(intersects[0]);
    bool secondInFront = inFrontOfDribbler(intersects[1]);
    //Check where both points are on the circle:
    if(firstInFront && secondInFront){
      //no intersections as both pass in front of the robot
      return {};
    }
    if(!firstInFront && !secondInFront){
      //Both points intersect the outer hull and not the dribbler. So we can simply return
      return intersects;
    }
    //One point is in front and one point is behind the kick line.
    //As the circle is convex, this must mean there is an intersection between the two.
    Vector2 dribblerIntersect = *kickerLine.intersects(segment); //If this crashes there has to be a bug somewhere.
    //Check which of the intersections was invalid.
    if(firstInFront){
      return {dribblerIntersect,intersects[1]};
    }
    return {intersects[0],dribblerIntersect};
  }
  return {};//This should never be hit as circle intersection always hits atleast two.
}
std::vector<Vector2> RobotShape::intersects(const Ray &ray) const {
  std::vector<Vector2> intersects = circle.intersects(ray);
  if(intersects.empty()){
    //The ray is completely outside the circle.
    return {};
  } else if (intersects.size() == 1){
    auto intersect = ray.intersects(LineSegment(kickerLine)); // we need this info in all branches
    //Either the ray touches the circle, or (more likely) it started  inside of it.
    if(inFrontOfDribbler(intersects[0])){
      //The ray might still have hit the kicker by starting inside of the robot
      if(intersect){
        return {*intersect};
      }
      return {};
    }
    //If the intersection point is on the hull there is no problem. However, there might be another intersection at the kicker.
    if(intersect){
      if((ray.start()-*intersect).length2()<(ray.start()-intersects[0]).length2()){
        return {*intersect,intersects[0]};
      }
      intersects.push_back(*intersect); //TODO: this double counts now if the lines go through the exact corner. (need to write function that ignores end point intersections)
    }
    return intersects;
  } else if (intersects.size() == 2){
    bool firstInFront = inFrontOfDribbler(intersects[0]);
    bool secondInFront = inFrontOfDribbler(intersects[1]);
    //Check where both points are on the circle:
    if(firstInFront && secondInFront){
      //no intersections as both pass in front of the robot
      return {};
    }
    if(!firstInFront && !secondInFront){
      //Both points intersect the outer hull and not the dribbler. So we can simply return
      return intersects;
    }
    //One point is in front and one point is behind the kick line.
    //As the circle is convex, this must mean there is an intersection between the two.
    Vector2 dribblerIntersect = *kickerLine.intersects(ray); //If this
    //Check which of the intersections was invalid.
    if(firstInFront){
      return {dribblerIntersect,intersects[1]};
    }
    return {intersects[0],dribblerIntersect};
  }
  return {};//This should never be hit as circle intersection always hits atleast two.
}
Angle RobotShape::angle() const {
  return orientation;
}
const Vector2 &RobotShape::pos() const {
  return circle.center();
}
Vector2 RobotShape::centerOfKickerPos() const {
  return (kickerLine.start()+kickerLine.end())*0.5;
}
LineSegment RobotShape::kicker() const {
  return LineSegment(kickerLine);
}
