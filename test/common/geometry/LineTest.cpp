//
// Created by rolf on 19-4-19.
//

#include <gtest/gtest.h>
#include <math/geometry/Line.h>
#include <math/geometry/LineSegment.h>
#include <math/geometry/Vector2.h>
TEST(LineTests, direction) {
    Vector2 v1(0.0, 0.0), v2(1.0, 1.0), v3(0.0, 0.0);
    Line l1(v1, v2), l2(v2, v1);

    EXPECT_EQ(l1.direction(), v2);
    EXPECT_EQ(l2.direction(), v2 * -1.0);
    EXPECT_NE(l1.direction(), l2.direction());
    Line l3(v1, v3);
    EXPECT_TRUE(l3.isPoint());
    EXPECT_FALSE(l1.isPoint());
    EXPECT_FALSE(l2.isPoint());
    Vector2 v4(0.0, 0.0), v5(0.0, 10.0), v6(2.0, 0.0), v7(2.0, 9.0);
    Vector2 v8(2.0, 0.0), v9(3.0, 10.0), v10(0.0 + std::numeric_limits<double>::epsilon(), 10);
    Line l4(v4, v5), l5(v6, v7), l11(v4, v8);
    LineSegment l6(v4, v8), l7(v5, v9), l9(v4, v5), l10(v6, v7);
    Line l8(v4, v10);
    EXPECT_TRUE(l4.isParallel(l5));
    EXPECT_TRUE(l5.isParallel(l4));
    EXPECT_TRUE(l6.isParallel(l7));
    EXPECT_TRUE(l7.isParallel(l6));
    EXPECT_FALSE(l1.isParallel(l4));
    EXPECT_FALSE(l1.isParallel(l5));
    EXPECT_FALSE(l1.isParallel(l6));
    EXPECT_FALSE(l1.isParallel(l7));

    EXPECT_FALSE(l4.isParallel(l6));
    EXPECT_FALSE(l5.isParallel(l6));
    EXPECT_FALSE(l6.isParallel(l4));
    EXPECT_FALSE(l6.isParallel(l5));
    EXPECT_FALSE(l4.isParallel(l7));
    EXPECT_FALSE(l5.isParallel(l7));
    EXPECT_FALSE(l7.isParallel(l4));
    EXPECT_FALSE(l7.isParallel(l5));
    EXPECT_FALSE(l4.isParallel(l8));

    EXPECT_TRUE(l9.isParallel(l10));
    EXPECT_TRUE(l10.isParallel(l9));
    EXPECT_FALSE(l9.isParallel(l6));
    EXPECT_FALSE(l6.isParallel(l9));

    EXPECT_TRUE(l9.isParallel(l5));
    EXPECT_TRUE(l10.isParallel(l4));
    EXPECT_FALSE(l9.isParallel(l11));
    EXPECT_TRUE(l6.isParallel(l11));
    EXPECT_FALSE(l6.isParallel(l8));
}
TEST(LineTests, slopeAndIntercept) {
    Vector2 Av(1.0, 1.0), Bv(2.0, 2.0), Cv(2.0, 3.0), Dv(1.0, 4.0), Ev(2.0, 0.0);
    Line D(Av, Bv), E(Av, Cv), H(Av, Ev);
    LineSegment F(Av, Dv), G(Dv, Av);
    Line Dcopy(Bv, Av), Ecopy(Cv, Av), Hcopy(Ev, Av);

    // test normal slopes
    EXPECT_DOUBLE_EQ(D.slope(), 1.0);
    EXPECT_DOUBLE_EQ(E.slope(), 2.0);
    EXPECT_DOUBLE_EQ(H.slope(), -1.0);
    // test vertical lines giving back numeric limits
    EXPECT_DOUBLE_EQ(F.slope(), std::numeric_limits<double>::infinity());
    EXPECT_DOUBLE_EQ(G.slope(), -std::numeric_limits<double>::infinity());
    EXPECT_FALSE(D.isVertical());
    EXPECT_FALSE(E.isVertical());
    EXPECT_FALSE(H.isVertical());
    EXPECT_TRUE(F.isVertical());
    EXPECT_TRUE(G.isVertical());
    // make sure the functions are commutative
    EXPECT_DOUBLE_EQ(D.slope(), Dcopy.slope());
    EXPECT_DOUBLE_EQ(E.slope(), Ecopy.slope());
    EXPECT_DOUBLE_EQ(H.slope(), Hcopy.slope());

    // calculate the intercept for normal cases and check commutativity
    EXPECT_DOUBLE_EQ(D.intercept(), 0.0);
    EXPECT_DOUBLE_EQ(E.intercept(), -1.0);
    EXPECT_DOUBLE_EQ(H.intercept(), 2.0);
    EXPECT_DOUBLE_EQ(D.intercept(), Dcopy.intercept());
    EXPECT_DOUBLE_EQ(E.intercept(), Ecopy.intercept());
    EXPECT_DOUBLE_EQ(H.intercept(), Hcopy.intercept());

    EXPECT_DOUBLE_EQ(F.intercept(), -std::numeric_limits<double>::infinity());
    EXPECT_DOUBLE_EQ(G.intercept(), std::numeric_limits<double>::infinity());

    EXPECT_DOUBLE_EQ(D.coefficients().first, D.slope());
    EXPECT_DOUBLE_EQ(E.coefficients().first, E.slope());
    EXPECT_DOUBLE_EQ(F.coefficients().first, F.slope());
    EXPECT_DOUBLE_EQ(G.coefficients().first, G.slope());
    EXPECT_DOUBLE_EQ(H.coefficients().first, H.slope());
    EXPECT_DOUBLE_EQ(D.coefficients().second, D.intercept());
    EXPECT_DOUBLE_EQ(E.coefficients().second, E.intercept());
    EXPECT_DOUBLE_EQ(F.coefficients().second, F.intercept());
    EXPECT_DOUBLE_EQ(G.coefficients().second, G.intercept());
    EXPECT_DOUBLE_EQ(H.coefficients().second, H.intercept());
}

TEST(LineTests, distanceToPoint) {
    Vector2 A(0.0, 0.0), B(2.0, 2.0);
    Vector2 point1(2.0, 0.0), point2(4.0, 6.0);
    Line l1(A, B);
    LineSegment l2(A, B);

    EXPECT_EQ(l1.distanceTo(point1), sqrt(2.0));
    EXPECT_EQ(l2.distanceTo(point1), sqrt(2.0));
    EXPECT_EQ(l1.distanceTo(point2), sqrt(2.0));
    EXPECT_EQ(l2.distanceTo(point2), sqrt(20.0));

    Vector2 shouldProj1(1.0, 1.0), shouldProj2(5.0, 5.0);
    EXPECT_EQ(l1.project(point1), shouldProj1);
    EXPECT_EQ(l2.project(point1), shouldProj1);
    EXPECT_EQ(l1.project(point2), shouldProj2);
    EXPECT_EQ(l2.project(point2), B);
}
TEST(LineTests, pointOnLine) {
    Vector2 A(1.0, 1.0), B(3.0, 1.0), C(1.0, 3.0), D(3.0, 3.0);
    Line l1(A, B), l2(A, C), l3(A, D);
    LineSegment ls1(A, B), ls2(A, C), ls3(A, D);
    Vector2 point1(2.0, 1.0), point2(4.0, 1.0);
    EXPECT_TRUE(l1.hits(point1));
    EXPECT_TRUE(l1.hits(point2));
    EXPECT_TRUE(ls1.hits(point1));
    EXPECT_FALSE(ls1.hits(point2));

    // check the original points
    EXPECT_TRUE(l1.hits(B));
    EXPECT_TRUE(ls1.hits(B));
    EXPECT_TRUE(l1.hits(A));
    EXPECT_TRUE(ls1.hits(A));

    Vector2 point3(1.0, 2.0), point4(1.0, 4.0);
    EXPECT_TRUE(l2.hits(point3));
    EXPECT_TRUE(l2.hits(point4));
    EXPECT_TRUE(ls2.hits(point3));
    EXPECT_FALSE(ls2.hits(point4));  // check the original points

    EXPECT_TRUE(l2.hits(C));
    EXPECT_TRUE(ls2.hits(C));
    EXPECT_TRUE(l2.hits(A));
    EXPECT_TRUE(ls2.hits(A));

    Vector2 point5(2.0, 2.0), point6(4.0, 4.0);
    EXPECT_TRUE(l3.hits(point5));
    EXPECT_TRUE(l3.hits(point6));
    EXPECT_TRUE(ls3.hits(point5));
    EXPECT_FALSE(ls3.hits(point6));  // check the original points

    EXPECT_TRUE(l3.hits(D));
    EXPECT_TRUE(ls3.hits(D));
    EXPECT_TRUE(l3.hits(A));
    EXPECT_TRUE(ls3.hits(A));

    Vector2 point7(0.0, 1.0), point8(0.0, 2.0);
    EXPECT_FALSE(ls1.hits(point7));
    EXPECT_FALSE(ls1.hits(point8));
}

TEST(LineTests, Intersections) {
    Vector2 P1(0.0, 0.0), P2(1.0, 1.0), P3(4.0, 0.0), P4(0.0, 4.0), P5(4.0, 4.0);
    Line L1(P1, P2), L2(P3, P4), L3(P3, P5), L4(P1, P5), L5(P1, P4);
    LineSegment LS1(P1, P2), LS2(P3, P4), LS3(P3, P5), LS4(P1, P5), LS5(P1, P4);
    Vector2 intersect(2.0, 2.0);

    // Test constructors
    EXPECT_EQ(Line(LS1).start(), L1.start());
    EXPECT_EQ(Line(LS1).end(), L1.end());
    EXPECT_NE(Line(LS1).start(), L2.start());
    EXPECT_NE(Line(LS1).end(), L2.end());
    EXPECT_EQ(LineSegment(L1).start(), LS1.start());
    EXPECT_EQ(LineSegment(L1).end(), LS1.end());

    EXPECT_NE(LineSegment(L1).start(), LS2.start());
    EXPECT_NE(LineSegment(L1).end(), LS2.end());

    ASSERT_NE(L1.intersects(L2), std::nullopt);
    ASSERT_TRUE(L1.doesIntersect(L2));
    EXPECT_EQ(*L1.intersects(L2), intersect);
    // test converse
    ASSERT_NE(L2.intersects(L1), std::nullopt);
    ASSERT_TRUE(L2.doesIntersect(L1));
    EXPECT_EQ(*L2.intersects(L1), intersect);

    ASSERT_EQ(LS1.intersects(LS2), std::nullopt);
    ASSERT_FALSE(LS1.doesIntersect(LS2));
    // test converse
    ASSERT_EQ(LS2.intersects(LS1), std::nullopt);
    ASSERT_FALSE(LS2.doesIntersect(LS1));

    ASSERT_NE(L2.intersects(L4), std::nullopt);
    ASSERT_TRUE(L2.doesIntersect(L4));
    EXPECT_EQ(*L2.intersects(L4), intersect);
    // test converse
    ASSERT_NE(L4.intersects(L2), std::nullopt);
    ASSERT_TRUE(L4.doesIntersect(L2));
    EXPECT_EQ(*L4.intersects(L2), intersect);

    ASSERT_NE(LS2.intersects(LS4), std::nullopt);
    ASSERT_TRUE(LS2.doesIntersect(LS4));
    EXPECT_EQ(*LS2.intersects(LS4), intersect);
    // test converse
    ASSERT_NE(LS4.intersects(LS2), std::nullopt);
    ASSERT_TRUE(LS4.doesIntersect(LS2));
    EXPECT_EQ(*LS4.intersects(LS2), intersect);

    ASSERT_NE(L3.intersects(L4), std::nullopt);
    ASSERT_TRUE(L3.doesIntersect(L4));
    EXPECT_EQ(*L3.intersects(L4), P5);
    // test converse
    ASSERT_NE(L4.intersects(L3), std::nullopt);
    ASSERT_TRUE(L4.doesIntersect(L3));
    EXPECT_EQ(*L4.intersects(L3), P5);

    ASSERT_NE(LS3.intersects(LS4), std::nullopt);
    ASSERT_TRUE(LS3.doesIntersect(LS4));
    EXPECT_EQ(*LS3.intersects(LS4), P5);
    // test converse
    ASSERT_NE(LS4.intersects(LS3), std::nullopt);
    ASSERT_TRUE(LS4.doesIntersect(LS3));
    EXPECT_EQ(*LS4.intersects(LS3), P5);

    EXPECT_EQ(L3.intersects(L5), std::nullopt);
    EXPECT_FALSE(L3.doesIntersect(L5));
    // test converse
    EXPECT_EQ(L5.intersects(L3), std::nullopt);
    EXPECT_FALSE(L5.doesIntersect(L3));

    EXPECT_EQ(LS3.intersects(LS5), std::nullopt);
    EXPECT_FALSE(LS3.doesIntersect(LS5));
    // test converse
    EXPECT_EQ(LS5.intersects(LS3), std::nullopt);
    EXPECT_FALSE(LS5.doesIntersect(LS3));

    EXPECT_EQ(L2.intersects(LS1), std::nullopt);
    EXPECT_FALSE(L2.doesIntersect(LS1));
    // two collinear lines
    LineSegment one(Vector2(0, 0), Vector2(1, 1));
    LineSegment two(Vector2(2, 2), Vector2(3, 3));
    LineSegment three(Vector2(1, 1), Vector2(2, 2));
    LineSegment four(Vector2(0, 0), Vector2(4, 4));
    EXPECT_FALSE(one.doesIntersect(two));
    EXPECT_FALSE(two.doesIntersect(one));
    EXPECT_TRUE(one.doesIntersect(three));
    EXPECT_TRUE(three.doesIntersect(one));
    EXPECT_TRUE(three.doesIntersect(two));
    EXPECT_TRUE(two.doesIntersect(three));

    EXPECT_TRUE(four.doesIntersect(three));
    EXPECT_TRUE(four.doesIntersect(one));
    EXPECT_TRUE(four.doesIntersect(two));
    EXPECT_TRUE(three.doesIntersect(four));
    EXPECT_TRUE(one.doesIntersect(four));
    EXPECT_TRUE(two.doesIntersect(four));

    EXPECT_EQ(one.intersects(three), Vector2(1, 1));
    EXPECT_EQ(three.intersects(one), Vector2(1, 1));

    LineSegment a(Vector2(0, 0), Vector2(2, 2));
    LineSegment b(Vector2(1, 1), Vector2(3, 3));
    Line la(a);
    Line lb(b);
    EXPECT_TRUE(a.doesIntersect(b));
    EXPECT_TRUE(b.doesIntersect(a));
    EXPECT_NE(a.intersects(b), std::nullopt);
    EXPECT_NE(a.intersects(b.reversed()), std::nullopt);
    EXPECT_NE((a.reversed()).intersects(b), std::nullopt);
    EXPECT_NE((a.reversed()).intersects(b.reversed()), std::nullopt);
}
TEST(LineTests, IntersectionsDifferentTypes) {
    Vector2 P1(0.0, 0.0), P2(2.0, 2.0), P3(2.0, 0.0), P4(0.0, 2.0);
    Vector2 middle(1.0, 1.0);
    Line L1(P1, P2), L2(P3, P4);
    LineSegment LS1(P1, P2), LS2(P3, P4);

    // no special things, just normal intersection. Should work as expected
    ASSERT_NE(LS1.intersects(L2), std::nullopt);
    ASSERT_EQ(*LS1.intersects(L2), middle);
    ASSERT_NE(L2.intersects(LS1), std::nullopt);
    ASSERT_EQ(*L2.intersects(LS1), middle);
    ASSERT_NE(LS2.intersects(L1), std::nullopt);
    ASSERT_EQ(*LS2.intersects(L1), middle);
    ASSERT_NE(L1.intersects(LS2), std::nullopt);
    ASSERT_EQ(*L1.intersects(LS2), middle);

    EXPECT_TRUE(LS1.doesIntersect(L2));
    EXPECT_TRUE(LS2.doesIntersect(L1));
    EXPECT_TRUE(L2.doesIntersect(LS1));
    EXPECT_TRUE(L1.doesIntersect(LS2));

    // Lies in the extension of the line
    Vector2 R1(0.0, 0.0), R2(1.0, 0.0), R3(0.0, -4.0), R4(8.0, 4.0);
    Line A1(R1, R2), A2(R3, R4);
    LineSegment AS1(R1, R2), AS2(R3, R4);

    EXPECT_EQ(AS1.intersects(A2), std::nullopt);
    EXPECT_FALSE(AS1.doesIntersect(A2));
    EXPECT_NE(A1.intersects(AS2), std::nullopt);
    EXPECT_TRUE(A1.doesIntersect(AS2));
    EXPECT_EQ(*A1.intersects(AS2), Vector2(4.0, 0.0));

    // edge cases
    Vector2 R5(1.0, 3.0), R6(1.0, -1.0), R7(2.0, 0.0), R8(3.0, 0.0);
    Line A3(R5, R6), A4(R2, R7), A5(R7, R8);
    LineSegment AS3(R5, R6), AS4(R2, R7), AS5(R7, R8);

    ASSERT_NE(AS1.intersects(AS4), std::nullopt);
    EXPECT_TRUE(AS1.doesIntersect(AS4));
    EXPECT_EQ(*AS1.intersects(AS4), R2);
}

TEST(LineTests, reverse) {
    Vector2 A(1, 1), B(2, 2);
    LineSegment x(A, B);
    EXPECT_EQ(x.start(), A);
    EXPECT_EQ(x.end(), B);
    x.reverse();
    EXPECT_EQ(x.start(), B);
    EXPECT_EQ(x.end(), A);

    LineSegment y(A, B);
    EXPECT_EQ(y.start(), A);
    EXPECT_EQ(y.end(), B);
    LineSegment z = y.reversed();
    EXPECT_EQ(z.start(), B);
    EXPECT_EQ(z.end(), A);
}

TEST(LineTests, center) {
    Vector2 A(1, 2), B(3, 6);
    LineSegment line(A, B);
    EXPECT_EQ(line.center().x(), 2);
    EXPECT_EQ(line.center().y(), 4);
}