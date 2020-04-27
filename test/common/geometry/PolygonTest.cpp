//
// Created by rolf on 14-5-19.
//

#include <gtest/gtest.h>
#include <math/geometry/Polygon.h>
#include <math/geometry/Line.h>

TEST(PolygonTest, constructors) {
    Vector2 leftCorner(1.0, 1.0), rightBottom(3.0, 1.0), rightTop(3.0, 4.0), leftTop(1.0, 4.0);
  const std::vector<Vector2>& corners={leftCorner,rightBottom,rightTop,leftTop};
  Quadrilateral rect(corners);
  Quadrilateral rect2(leftCorner, rightBottom, rightTop, leftTop);
  EXPECT_EQ(leftCorner, rect2[0]);
  EXPECT_EQ(rightBottom, rect2[1]);
  EXPECT_EQ(rightTop, rect2[2]);
  EXPECT_EQ(leftTop, rect2[3]);
  EXPECT_EQ(rect[0], rect2[0]);
  EXPECT_EQ(rect[1], rect2[1]);
  EXPECT_EQ(rect[2], rect2[2]);
  EXPECT_EQ(rect[3], rect2[3]);
  EXPECT_EQ(rect[0], corners[0]);
  EXPECT_EQ(rect[1], corners[1]);
  EXPECT_EQ(rect[2], corners[2]);
  EXPECT_EQ(rect[3], corners[3]);
}

TEST(PolygonTest, simpleFunctions) {
    Vector2 leftCorner(1.0, 1.0), rightBottom(3.0, 1.0), rightTop(3.0, 4.0), leftTop(1.0, 4.0);
    Quadrilateral rect2(leftCorner, rightBottom, rightTop, leftTop);
    Triangle triangle(leftCorner, rightBottom, leftTop);

    ASSERT_EQ(rect2.amountOfVertices(), 4);
    ASSERT_EQ(triangle.amountOfVertices(), 3);

    Vector2 moveBy(-3.7,2.5);
    Triangle copy = triangle;
    copy.move(moveBy);
  for (std::size_t kI = 0; kI < copy.amountOfVertices(); ++kI) {
    EXPECT_EQ(copy[kI],triangle[kI]+moveBy);
  }
  Quadrilateral quadCopy = rect2;
  quadCopy.move(moveBy);
  for (std::size_t kI = 0; kI < quadCopy.amountOfVertices(); ++kI) {
    EXPECT_EQ(quadCopy[kI],rect2[kI]+moveBy);
  }
}
TEST(PolygonTest, isSimple) {
    Vector2 leftCorner(1.0, 1.0), rightBottom(3.0, 1.0), rightTop(3.0, 4.0), leftTop(1.0, 4.0);
    // should be simple
    Polygon<4> rect2(leftCorner, rightBottom, rightTop, leftTop);
    Polygon<4> rect3(leftCorner, leftTop, rightTop, rightBottom);

    Polygon<3> triangle(leftCorner, rightBottom, leftTop);

    EXPECT_TRUE(rect2.isSimple());
    EXPECT_TRUE(rect3.isSimple());
    EXPECT_TRUE(triangle.isSimple());
    // all non-simple cases
    Polygon<4> rect4(leftCorner, rightBottom, leftTop, rightTop);
    Polygon<4> rect5(leftCorner, leftTop, rightBottom, rightTop);
    Polygon<4> rect6(leftCorner, rightTop, leftTop, rightBottom);
    Polygon<4> rect7(leftCorner, rightTop, rightBottom, leftTop);
    EXPECT_FALSE(rect4.isSimple());
    EXPECT_FALSE(rect5.isSimple());
    EXPECT_FALSE(rect6.isSimple());
    EXPECT_FALSE(rect7.isSimple());
}
TEST(PolygonTest, isConvex) {
    Vector2 leftCorner(1.0, 1.0), rightBottom(3.0, 1.0), rightTop(3.0, 4.0), leftTop(1.0, 4.0), convexPoint(4.0, 2.0), nonConvexPoint(2.0, 2.0);
    Polygon<5> convexPentagon(leftCorner, rightBottom, convexPoint, rightTop, leftTop);
    Polygon<5> convexPentagonCCW(leftCorner, leftTop, rightTop, convexPoint, rightBottom);
    Polygon<5> nonConvexPentagon(leftCorner, rightBottom, nonConvexPoint, rightTop, leftTop);
    Polygon<5> nonConvexPentagonCCW(leftCorner, leftTop, rightTop, nonConvexPoint, rightBottom);
    // edge cases
    Vector2 convexPointLast(0.0, 2.0), nonConvexPointLast(0.0, 0.0), nonConvexPointLast2(2.0, 2.0);
    Polygon<5> P1Last(leftCorner, rightBottom, rightTop, leftTop, convexPointLast);
    Polygon<5> P1First(convexPointLast, leftCorner, rightBottom, rightTop, leftTop);
    Polygon<5> P2Last(leftCorner, rightBottom, rightTop, leftTop, nonConvexPointLast);
    Polygon<5> P2First(nonConvexPointLast, leftCorner, rightBottom, rightTop, leftTop);
    Polygon<5> P3Last(leftCorner, rightBottom, rightTop, leftTop, nonConvexPointLast2);
    Polygon<5> P3First(nonConvexPointLast2, leftCorner, rightBottom, rightTop, leftTop);
    Polygon<8> P4(leftCorner, Vector2(2.0, 1.0), rightBottom, Vector2(3.0, 2.0), rightTop, Vector2(2.0, 4.0), leftTop, Vector2(1.0, 2.0));

    std::vector<Polygon<5>> allExamples = {convexPentagon, convexPentagonCCW, nonConvexPentagon, nonConvexPentagonCCW, P1Last, P1First, P2First, P2Last, P3First, P3Last};
    // P1Last,P1First,;
    // convex calculation only makes sense if the examples we use are actually valid
    for (const Polygon<5> &example : allExamples) {
        ASSERT_TRUE(example.isSimple());
    }
    ASSERT_TRUE(P4.isSimple());
    EXPECT_TRUE(convexPentagon.isConvex());
    EXPECT_TRUE(convexPentagonCCW.isConvex());
    EXPECT_FALSE(nonConvexPentagon.isConvex());
    EXPECT_FALSE(nonConvexPentagonCCW.isConvex());

    EXPECT_TRUE(P1First.isConvex());
    EXPECT_TRUE(P1Last.isConvex());
    EXPECT_FALSE(P2First.isConvex());
    EXPECT_FALSE(P2Last.isConvex());
    EXPECT_FALSE(P3First.isConvex());
    EXPECT_FALSE(P3Last.isConvex());
    EXPECT_TRUE(P4.isConvex());
}
TEST(PolygonTest, boundaryTests) {
    Vector2 leftCorner(1.0, 1.0), rightBottom(3.0, 1.0), rightTop(3.0, 4.0), leftTop(1.0, 4.0);
    double width = 2.0;
    double height = 3.0;
    Polygon<4> rect2(leftCorner, rightBottom, rightTop, leftTop);
    Polygon<4> other(leftCorner, rightBottom, Vector2(3.0,3.0),leftTop);
    std::vector<Polygon<4>> objects = { rect2, other};
    for (const Polygon<4> &obj : objects) {
        std::array<LineSegment,4> lines = obj.getBoundary();
        for (int i = 0; i < lines.size(); ++i) {
            EXPECT_EQ(lines[i].start(), obj[i]);
            EXPECT_EQ(lines[i].end(), i == lines.size() - 1 ? obj[0] : obj[i + 1]);
        }
        for (const LineSegment &line : lines) {
            EXPECT_TRUE(obj.isOnBoundary(line.start()));
            EXPECT_TRUE(obj.isOnBoundary(line.end()));
            EXPECT_TRUE(obj.isOnBoundary((line.start() + line.end()) * 0.5));
        }
        double lenSum = 0;
        for (const LineSegment &line : lines) {
            lenSum += line.length();
        }
        EXPECT_EQ(lenSum, obj.perimeterLength());
    }
    EXPECT_EQ(rect2.perimeterLength(), 2 * (width + height));
    EXPECT_EQ(other.perimeterLength(), 7.0 + sqrt(5.0));
}
TEST(PolygonTest, areas) {
    double width = 4.0;
    double height = 4.0;
    double sideOffset = 1.0;
    Vector2 A(0.0, 0.0), B(width, 0.0), C(width + sideOffset, height), D(sideOffset, height), E(width, height), F(0.0, height);
    Polygon<4> parallelogram(A, B, C, D);
    Polygon<4> rectangle(A, B, E, F);
    Polygon<3> triangleA(A, B, C), triangleB(A, B, D), triangleC(A, B, E), triangleD(A, B, F);
    ASSERT_TRUE(parallelogram.isSimple());
    ASSERT_TRUE(rectangle.isSimple());
    EXPECT_EQ(parallelogram.area(), width * height);
    EXPECT_EQ(rectangle.area(), width * height);
    EXPECT_EQ(triangleA.area(), width * height * 0.5);
    EXPECT_EQ(triangleB.area(), width * height * 0.5);
    EXPECT_EQ(triangleC.area(), width * height * 0.5);
    EXPECT_EQ(triangleD.area(), width * height * 0.5);
    // below should hold for any random 3 points (could randomly generate some later perhaps)
    Vector2 G(-3.57, 0.4);
    Polygon<3> triangleE(A, D, G);
    Line l(A, D);
    EXPECT_EQ(triangleE.area(), 0.5 * (l.start() - l.end()).length() * l.distanceTo(G));
}
TEST(PolygonTest, intersections) {
    Vector2 leftCorner(1.0, 1.0), rightBottom(3.0, 1.0), rightTop(3.0, 3.0), leftTop(1.0, 3.0);
    Vector2 OP1(0.5, 2.0), OP2(2.0, 0.5), OP3(3.5, 2.0), OP4(2.0, 3.5);
    LineSegment L1(OP1, OP2), L2(OP2, OP3), L3(OP3, OP4), L4(OP4, OP1);
    Polygon<4> rect(leftCorner, rightBottom, rightTop, leftTop);

    EXPECT_TRUE(rect.doesIntersect(L1));
    EXPECT_TRUE(rect.doesIntersect(L2));
    EXPECT_TRUE(rect.doesIntersect(L3));
    EXPECT_TRUE(rect.doesIntersect(L4));
    EXPECT_EQ(rect.intersects(L1).size(), 2);
    EXPECT_EQ(rect.intersects(L2).size(), 2);
    EXPECT_EQ(rect.intersects(L3).size(), 2);
    EXPECT_EQ(rect.intersects(L4).size(), 2);
    Vector2 OP5(2.0, 2.0);
    LineSegment L5(OP1, OP5);
    EXPECT_TRUE(rect.doesIntersect(L5));
    EXPECT_EQ(rect.intersects(L5).size(), 1);

    // edge cases
    Vector2 OPE1(0.0, 2.0), OPE2(2.0, 0.0), OPE3(4.0, 2.0), OPE4(2.0, 4.0);
    LineSegment LE1(OPE1, OPE2), LE2(OPE2, OPE3), LE3(OPE3, OPE4), LE4(OPE4, OPE1);
    EXPECT_TRUE(rect.doesIntersect(LE1));
    EXPECT_TRUE(rect.doesIntersect(LE2));
    EXPECT_TRUE(rect.doesIntersect(LE3));
    EXPECT_TRUE(rect.doesIntersect(LE4));
    ASSERT_EQ(rect.intersects(LE1).size(), 1);
    ASSERT_EQ(rect.intersects(LE2).size(), 1);
    ASSERT_EQ(rect.intersects(LE3).size(), 1);
    ASSERT_EQ(rect.intersects(LE4).size(), 1);
    EXPECT_EQ(rect.intersects(LE1)[0], leftCorner);
    EXPECT_EQ(rect.intersects(LE2)[0], rightBottom);
    EXPECT_EQ(rect.intersects(LE3)[0], rightTop);
    EXPECT_EQ(rect.intersects(LE4)[0], leftTop);
}
TEST(PolygonTest, contains) {
    Vector2 leftCorner(1.0, 1.0), rightBottom(3.0, 1.0), rightTop(3.0, 4.0), leftTop(1.0, 4.0);
    Polygon<4> rect(leftCorner, rightBottom, rightTop, leftTop);
    Polygon<3> triangle(leftCorner, rightBottom, leftTop);

    Vector2 P1(2.0, 2.0), P2(2.5, 2.5), P3(0.0, 2.0), P4(0.1, 0.1), P5(4.0, 2.0), P6(2.0, 6.0), P7(2.0, 0.0);
    ASSERT_TRUE(rect.contains(P1));
    ASSERT_TRUE(rect.contains(P2));
    ASSERT_FALSE(rect.contains(P3));
    ASSERT_FALSE(rect.contains(P4));
    ASSERT_FALSE(rect.contains(P5));
    ASSERT_FALSE(rect.contains(P6));
    ASSERT_FALSE(rect.contains(P7));

    ASSERT_TRUE(triangle.contains(P1));
    ASSERT_FALSE(triangle.contains(P2));
    ASSERT_FALSE(triangle.contains(P3));
    ASSERT_FALSE(triangle.contains(P4));
    ASSERT_FALSE(triangle.contains(P5));
    ASSERT_FALSE(triangle.contains(P6));
    ASSERT_FALSE(triangle.contains(P7));
}
TEST(PolygonTest, centroid) {
    Vector2 A(5.0, 3.0), B(6.0, 1.0), C(7.0, 8.0), D(1.0, 2.0);
    Polygon<3> triangle(A, B, C), triangle2(A, B, D);
    Polygon<4> quadrilateral(A, B, C, D);
    Vector2 centroid1(6, 4), centroid2(4.0, 2.0), centroid3(4 + 8.0 / 9.0, 4 + 2.0 / 9.0);
    ASSERT_EQ(triangle.centroid(), centroid1);
    ASSERT_EQ(triangle2.centroid(), centroid2);
    ASSERT_TRUE(quadrilateral.isSimple());
    ASSERT_EQ(quadrilateral.centroid(), centroid3);
}