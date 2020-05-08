//
// Created by emiel on 25-02-20.
//

#include <math/geometry/Circle.h>
#include <math/geometry/Line.h>
#include <math/geometry/LineSegment.h>
#include <math/geometry/Rectangle.h>
#include <gtest/gtest.h>
#include <cmath>
#include <math/geometry/BoundingBox2D.h>
#include <math/geometry/Ray.h>

TEST(CircleTests, instantiation) {
    // Test unit circle
    Circle unitCircle;
    EXPECT_DOUBLE_EQ(unitCircle.center().x(), 0);
    EXPECT_DOUBLE_EQ(unitCircle.center().y(), 0);
    EXPECT_DOUBLE_EQ(unitCircle.radius(), 1.0);
    // Normal instantiation
    Circle circle1({10, 20}, 2);
    EXPECT_DOUBLE_EQ(circle1.center().x(), 10);
    EXPECT_DOUBLE_EQ(circle1.center().y(), 20);
    EXPECT_DOUBLE_EQ(circle1.radius(), 2.0);
    // Copy instantiation
    Circle circle2(circle1);
    EXPECT_DOUBLE_EQ(circle2.center().x(), 10);
    EXPECT_DOUBLE_EQ(circle2.center().y(), 20);
    EXPECT_DOUBLE_EQ(circle2.radius(), 2.0);
    // Negative radius
    Circle circle3({30, 40}, -4);
    EXPECT_DOUBLE_EQ(circle3.center().x(), 30);
    EXPECT_DOUBLE_EQ(circle3.center().y(), 40);
    EXPECT_DOUBLE_EQ(circle3.radius(), 4.0);
}

TEST(CircleTests, doesIntersectOrContainVector) {
    Circle circle({0, 0}, 1);
    // Inside the circle
    Vector2 vectorIn(0, 0);
    EXPECT_TRUE(circle.contains(vectorIn));
    // On the circle
    Vector2 vectorOn(1, 0);
    EXPECT_TRUE(circle.contains(vectorOn));
    // Outside the circle
    Vector2 vectorOut(10, 10);
    EXPECT_FALSE(circle.contains(vectorOut));
}

TEST(CircleTests, doesIntersectOrContainLine) {
    Circle circle({0, 0}, 1);
    // Inside the circle
    Line lineThrough({-1, 0}, {1, 0});
    EXPECT_TRUE(circle.doesIntersectOrContain(lineThrough));
    // On the circle
    Line lineOn({-1, 1}, {1, 1});
    EXPECT_TRUE(circle.doesIntersectOrContain(lineOn));
    // Outside the circle
    Line lineOut({-1, 2}, {1, 2});
    EXPECT_FALSE(circle.doesIntersectOrContain(lineOut));
}

TEST(CircleTests, doesIntersectOrContainLineSegment) {
    Circle circle({0, 0}, 1);
    // Inside the circle
    LineSegment lineThrough({-1, 0}, {1, 0});
    EXPECT_TRUE(circle.doesIntersectOrContain(lineThrough));
    // On the circle
    LineSegment lineOn({-1, 1}, {1, 1});
    EXPECT_TRUE(circle.doesIntersectOrContain(lineOn));
    // Outside the circle
    LineSegment lineOut({-1, 2}, {1, 2});
    EXPECT_FALSE(circle.doesIntersectOrContain(lineOut));
}

TEST(CircleTests, doesIntersectOrContainCircle) {
    Circle circle({0, 0}, 1);
    // Inside the circle
    Circle circleThrough({0, 0}, 1);
    EXPECT_TRUE(circle.doesIntersectOrContain(circleThrough));
    // On the circle
    Circle circleOn({2, 0}, 1);
    EXPECT_TRUE(circle.doesIntersectOrContain(circleOn));
    // Outside the circle
    Circle circleOut({4, 0}, 1);
    EXPECT_FALSE(circle.doesIntersectOrContain(circleOut));
}

TEST(CircleTests, doesIntersectOrContainRectangle) {
    Circle circle({0, 0}, 1);
    // Around the circle
    Rectangle rectAround({2, 2}, {-2, -2});
    EXPECT_TRUE(circle.doesIntersectOrContain(rectAround));
    EXPECT_TRUE(circle.doesIntersectOrContain2(rectAround));
    // In the circle
    Rectangle rectIn({.2, .2}, {-.2, -.2});
    EXPECT_TRUE(circle.doesIntersectOrContain(rectIn));
    EXPECT_TRUE(circle.doesIntersectOrContain2(rectIn));
    // Through the circle
    Rectangle rectThrough({2, 2}, {0, 0});
    EXPECT_TRUE(circle.doesIntersectOrContain(rectThrough));
    EXPECT_TRUE(circle.doesIntersectOrContain2(rectThrough));
    // On the circle
    Rectangle rectOn({1, 1}, {2, 0});
    EXPECT_TRUE(circle.doesIntersectOrContain(rectOn));
    EXPECT_TRUE(circle.doesIntersectOrContain2(rectOn));
    // Outside the circle
    Rectangle rectOutside({4, 4}, {2, 2});
    EXPECT_FALSE(circle.doesIntersectOrContain(rectOutside));
    EXPECT_FALSE(circle.doesIntersectOrContain2(rectOutside));

    Rectangle rectSide({0.5, 0.5}, {2, 2});
    EXPECT_TRUE(circle.doesIntersectOrContain(rectSide));
    EXPECT_TRUE(circle.doesIntersectOrContain2(rectSide));
    Rectangle rectSide2({0.5 * sqrt(2), 0.5 * sqrt(2)}, {2, 2});
    EXPECT_TRUE(circle.doesIntersectOrContain(rectSide2));
    EXPECT_TRUE(circle.doesIntersectOrContain2(rectSide2));
    Rectangle rectSide3({1, 1}, {2, 2});
    EXPECT_FALSE(circle.doesIntersectOrContain(rectSide3));
    EXPECT_FALSE(circle.doesIntersectOrContain2(rectSide3));

    Rectangle rectAbove({-1, 2}, {1, 18});
    EXPECT_FALSE(circle.doesIntersectOrContain2(rectAbove));
    EXPECT_FALSE(circle.doesIntersectOrContain(rectAbove));
}

TEST(CircleTests, rectanglePerformance1) {
    Circle circle({0, 0}, 1);
    Rectangle rectThrough({2, 2}, {0, 0});
    for (int i = 0; i < 1e7; i++) circle.doesIntersectOrContain(rectThrough);
}

TEST(CircleTests, rectanglePerformance2) {
    Circle circle({0, 0}, 1);
    Rectangle rectThrough({2, 2}, {0, 0});
    for (int i = 0; i < 1e7; i++) circle.doesIntersectOrContain2(rectThrough);
}

TEST(CircleTests, projectTest) {
    Circle circle({0, 0}, 1);
    // Inside the circle
    Vector2 vectorIn(0.5, 0);
    Vector2 vectorInP = circle.project(vectorIn);
    EXPECT_DOUBLE_EQ(vectorInP.x(), 1);
    EXPECT_DOUBLE_EQ(vectorInP.y(), 0);
    // On the circle
    Vector2 vectorOn(1, 0);
    Vector2 vectorOnP = circle.project(vectorOn);
    EXPECT_DOUBLE_EQ(vectorOnP.x(), 1);
    EXPECT_DOUBLE_EQ(vectorOnP.y(), 0);
    // Outside the circle
    Vector2 vectorOut(2, 0);
    Vector2 vectorOutP = circle.project(vectorOut);
    EXPECT_DOUBLE_EQ(vectorOutP.x(), 1);
    EXPECT_DOUBLE_EQ(vectorOutP.y(), 0);
    // Exactly on the circle
    Vector2 vectorZero(0, 0);
    Vector2 vectorZeroP = circle.project(vectorZero);
    EXPECT_DOUBLE_EQ(vectorZeroP.x(), 1);
    EXPECT_DOUBLE_EQ(vectorZeroP.y(), 0);
    // Under a 45 degree angle
    Vector2 vectorAngle(2, 2);
    Vector2 vectorAngleP = circle.project(vectorAngle);
    EXPECT_DOUBLE_EQ(vectorAngleP.x(), std::sqrt(2) / 2);
    EXPECT_DOUBLE_EQ(vectorAngleP.y(), std::sqrt(2) / 2);
}

TEST(CircleTests, print) {
    Circle circle({1, 1}, 1);
    std::cout << circle << std::endl;
}
TEST(CircleTests, equalities) {
    Circle circle({1, 1}, 1);
    Circle circle2({1, 1}, 2);
    Circle circle3({1, 2}, 1);
    Circle copy = circle;
    EXPECT_TRUE(circle == copy);
    EXPECT_TRUE(copy == circle);
    EXPECT_FALSE(circle != copy);
    EXPECT_FALSE(copy != circle);

    EXPECT_FALSE(circle == circle2);
    EXPECT_FALSE(circle2 == circle);
    EXPECT_TRUE(circle != circle2);
    EXPECT_TRUE(circle2 != circle);

    EXPECT_FALSE(circle == circle3);
    EXPECT_FALSE(circle3 == circle);
    EXPECT_TRUE(circle != circle3);
    EXPECT_TRUE(circle3 != circle);

    EXPECT_FALSE(circle3 == circle2);
    EXPECT_FALSE(circle2 == circle3);
    EXPECT_TRUE(circle3 != circle2);
    EXPECT_TRUE(circle2 != circle3);
}
TEST(CircleTests,move){
  Circle x(Vector2(1,2),3);
  EXPECT_EQ(x.center(),Vector2(1,2));
  EXPECT_EQ(x.radius(),3);
  x.move(Vector2(1,2));
  EXPECT_EQ(x.center(),Vector2(2,4));
  EXPECT_EQ(x.radius(),3);
}
TEST(CircleTests,boundingBox){
  Circle x(Vector2(1,2),1);
  BoundingBox2D box = x.boundingBox();
  EXPECT_EQ(box.xMin(),0);
  EXPECT_EQ(box.xMax(),2);
  EXPECT_EQ(box.yMin(),1);
  EXPECT_EQ(box.yMax(),3);
}
TEST(CircleTests,lineIntersection){
  Circle x(Vector2(1,1),1);
  LineSegment oneNonTangent(Vector2(0,0),Vector2(1,1));
  double intersect=1-sqrt(2)*0.5;
  EXPECT_TRUE(x.doesIntersect(oneNonTangent));
  EXPECT_EQ(x.intersects(oneNonTangent).size(),1);
  EXPECT_EQ(Vector2(intersect,intersect),x.intersects(oneNonTangent)[0]);

  LineSegment twoNonTangent(Vector2(0,0),Vector2(2,2));
  EXPECT_TRUE(x.doesIntersect(twoNonTangent));
  EXPECT_EQ(x.intersects(twoNonTangent).size(),2);
  EXPECT_EQ(Vector2(intersect,intersect),x.intersects(twoNonTangent)[0]);
  EXPECT_EQ(Vector2(1+sqrt(2)*0.5,1+sqrt(2)*0.5),x.intersects(twoNonTangent)[1]);


  LineSegment tangent(Vector2(0,0),Vector2(0,2));
  LineSegment tangentTouch(Vector2(0,0),Vector2(0,1));
  for(const auto &segment : {tangent,tangentTouch}){
    EXPECT_TRUE(x.doesIntersect(segment));
    EXPECT_EQ(x.intersects(segment).size(),1);
    EXPECT_EQ(x.intersects(segment)[0],Vector2(0,1));
  }

  LineSegment inside(Vector2(0.5,0.5),Vector2(1.5,1.5));
  EXPECT_FALSE(x.doesIntersect(inside));
  EXPECT_TRUE(x.intersects(inside).empty());

  LineSegment outSide(Vector2(-1.0,-1.0),Vector2(0,0));
  EXPECT_FALSE(x.doesIntersect(outSide));
  EXPECT_TRUE(x.intersects(outSide).empty());

  LineSegment outsideNeverTouching(Vector2(-1.0,-10.0),Vector2(0,-9.0));
  EXPECT_FALSE(x.doesIntersect(outsideNeverTouching));
  EXPECT_TRUE(x.intersects(outsideNeverTouching).empty());
}
TEST(CircleTests,rayIntersection){
  Circle x(Vector2(1,1),1);
  Ray nonTangentNormal(Vector2(-1,-1),Vector2(0,0));
  double intersect=1-sqrt(2)*0.5;
  EXPECT_TRUE(x.doesIntersect(nonTangentNormal));
  EXPECT_EQ(x.intersects(nonTangentNormal).size(),2);
  EXPECT_DOUBLE_EQ(intersect,x.intersects(nonTangentNormal)[0].x());
  EXPECT_DOUBLE_EQ(intersect,x.intersects(nonTangentNormal)[0].y());
  EXPECT_EQ(Vector2(1+sqrt(2)*0.5,1+sqrt(2)*0.5),x.intersects(nonTangentNormal)[1]);

  Ray nonTangentAway(Vector2(0,0),Vector2(-1,-1));
  EXPECT_FALSE(x.doesIntersect(nonTangentAway));
  EXPECT_TRUE(x.intersects(nonTangentAway).empty());

  Ray tangent(Vector2(0,0),Vector2(0,2));
  EXPECT_TRUE(x.doesIntersect(tangent));
  EXPECT_EQ(x.intersects(tangent).size(),1);
  EXPECT_EQ(x.intersects(tangent)[0],Vector2(0,1));

  Ray inside(Vector2(1.5,1.5),Vector2(0.5,0.5));
  EXPECT_TRUE(x.doesIntersect(inside));
  EXPECT_EQ(x.intersects(inside).size(),1);
  EXPECT_DOUBLE_EQ(intersect,x.intersects(inside)[0].x());
  EXPECT_DOUBLE_EQ(intersect,x.intersects(inside)[0].y());

  Ray outsideNeverTouching(Vector2(-1.0,-10.0),Vector2(0,-9.0));
  EXPECT_FALSE(x.doesIntersect(outsideNeverTouching));
  EXPECT_TRUE(x.intersects(outsideNeverTouching).empty());
}