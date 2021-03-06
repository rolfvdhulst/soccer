//
// Created by rolf on 22-01-20.
//

#include <gtest/gtest.h>
#include <math/geometry/Line.h>
#include <math/geometry/LineSegment.h>
#include <math/geometry/Polygon.h>
#include <math/geometry/Rectangle.h>
static Rectangle rect(Vector2(-1, 1), Vector2(1, 2));
static Rectangle nullExample(Vector2(0, 0), Vector2(0, 0));
TEST(Rectangle, cohenCodes) {
    EXPECT_EQ(rect.CohenSutherlandCode(Vector2(0, 1.2)), 0x00);
    EXPECT_EQ(rect.CohenSutherlandCode(Vector2(-1, 1)), 0x00);
    EXPECT_EQ(rect.CohenSutherlandCode(Vector2(-1.5, 1.5)), 0x01);
    EXPECT_EQ(rect.CohenSutherlandCode(Vector2(1.5, 1.2)), 0x02);
    EXPECT_EQ(rect.CohenSutherlandCode(Vector2(1.5, 2)), 0x02);
    EXPECT_EQ(rect.CohenSutherlandCode(Vector2(-.5, 0)), 0x04);
    EXPECT_EQ(rect.CohenSutherlandCode(Vector2(-1, 0)), 0x04);
    EXPECT_EQ(rect.CohenSutherlandCode(Vector2(-1, 3)), 0x08);
    EXPECT_EQ(rect.CohenSutherlandCode(Vector2(-.5, 3)), 0x08);
    EXPECT_EQ(rect.CohenSutherlandCode(Vector2(-1.1, 0)), 0x05);
    EXPECT_EQ(rect.CohenSutherlandCode(Vector2(1.1, 0)), 0x06);
    EXPECT_EQ(rect.CohenSutherlandCode(Vector2(-1.5, 2.5)), 0x09);
    EXPECT_EQ(rect.CohenSutherlandCode(Vector2(1.5, 2.5)), 0x0A);
}

TEST(Rectangle, degenerateCohenCodes) {
    EXPECT_EQ(nullExample.CohenSutherlandCode(Vector2(0, 0)), 0x00);
    EXPECT_EQ(nullExample.CohenSutherlandCode(Vector2(1, 0)), 0x02);
    EXPECT_EQ(nullExample.CohenSutherlandCode(Vector2(-1, 0)), 0x01);
    EXPECT_EQ(nullExample.CohenSutherlandCode(Vector2(0, 1)), 0x08);
    EXPECT_EQ(nullExample.CohenSutherlandCode(Vector2(0, -.1)), 0x04);
}
TEST(Rectangle, contains) {
    EXPECT_TRUE(rect.contains(Vector2(0, 1.5)));
    EXPECT_TRUE(rect.contains(Vector2(-1, 1.5)));
    EXPECT_FALSE(rect.contains(Vector2(-2, 1.5)));
    EXPECT_TRUE(rect.contains(Vector2(1, 1.5)));
    EXPECT_FALSE(rect.contains(Vector2(2, 1.5)));

    EXPECT_TRUE(rect.contains(Vector2(0, 2)));
    EXPECT_TRUE(rect.contains(Vector2(0, 1)));
    EXPECT_FALSE(rect.contains(Vector2(0, 2.5)));
    EXPECT_FALSE(rect.contains(Vector2(0, 0.5)));

    EXPECT_FALSE(rect.contains(Vector2(-2, 2.5)));
    EXPECT_FALSE(rect.contains(Vector2(2, 0.5)));
    EXPECT_FALSE(rect.contains(Vector2(2, 2.5)));
    EXPECT_FALSE(rect.contains(Vector2(-2, 0.5)));
}
TEST(Rectangle, segmentIntersection) {
    auto results = nullExample.intersects(LineSegment(Vector2(1, 0), Vector2(-1, 0)));
    EXPECT_FALSE(results.empty());
    results = nullExample.intersects(LineSegment(Vector2(1, .1), Vector2(-1, 0)));
    EXPECT_TRUE(results.empty());
    EXPECT_FALSE(nullExample.doesIntersect(LineSegment(Vector2(1, .1), Vector2(-1, 0))));
    Vector2 v1(-1, 1);
    Vector2 v2(1, 2);
    auto res = rect.intersects(LineSegment(Vector2(2, 1.5), Vector2(-2, 1.5)));
    EXPECT_FALSE(res.empty());
    res = rect.intersects(LineSegment(Vector2(-2, .5), Vector2(2, 2.5)));
    EXPECT_FALSE(res.empty());
    EXPECT_TRUE(rect.doesIntersect(LineSegment(Vector2(-2, .5), Vector2(2, 2.5))));
    EXPECT_TRUE(res.size() == 2);
    for (const auto& point : res) {
        EXPECT_TRUE(point == v1 || point == v2);
    }
}

TEST(Rectangle, simpleFunctions) {
    Rectangle rectangle(Vector2(-1, -1), 2, 1);
    EXPECT_DOUBLE_EQ(rectangle.minX(), -1.0);
    EXPECT_DOUBLE_EQ(rectangle.minY(), -1.0);
    EXPECT_DOUBLE_EQ(rectangle.maxX(), 1.0);
    EXPECT_DOUBLE_EQ(rectangle.maxY(), 0.0);
    EXPECT_DOUBLE_EQ(rectangle.width(), 2.0);
    EXPECT_DOUBLE_EQ(rectangle.height(), 1.0);

    // We check if corners are properly defined to be clockwise ordered
    auto corners = rectangle.corners();
    auto lines = rectangle.lines();
    EXPECT_DOUBLE_EQ(corners[0].x(), -1.0);
    EXPECT_DOUBLE_EQ(corners[0].y(), -1.0);
    EXPECT_DOUBLE_EQ(corners[1].x(), -1.0);
    EXPECT_DOUBLE_EQ(corners[1].y(), 0.0);
    EXPECT_DOUBLE_EQ(corners[2].x(), 1.0);
    EXPECT_DOUBLE_EQ(corners[2].y(), 0.0);
    EXPECT_DOUBLE_EQ(corners[3].x(), 1.0);
    EXPECT_DOUBLE_EQ(corners[3].y(), -1.0);

    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(lines[i].start(), corners[i]);
        if (i < 3) {
            EXPECT_EQ(lines[i].end(), corners[i + 1]);
        } else {
            EXPECT_EQ(lines[i].end(), corners[0]);
        }
    }

    Vector2 centre = rectangle.center();
    EXPECT_DOUBLE_EQ(centre.x(), 0.0);
    EXPECT_DOUBLE_EQ(centre.y(), -0.5);


    std::cout << rectangle << std::endl;
}

TEST(Rectangle, LineIntersections) {
    Rectangle test(Vector2(0, 0), Vector2(3, 4));

    Line test1(Vector2(2, -1), Vector2(2, 5));

    auto intersects = test.intersects(test1);
    EXPECT_EQ(intersects.size(), 2);

    // Note the intersections are not ordered in any way!
    EXPECT_EQ(intersects[0], Vector2(2, 4));
    EXPECT_EQ(intersects[1], Vector2(2, 0));
    EXPECT_TRUE(test.doesIntersect(test1));

    Line test2(Vector2(4, -1), Vector2(4, 5));

    EXPECT_EQ(test.intersects(test2).size(), 0);
    EXPECT_FALSE(test.doesIntersect(test2));
    // Boundary points might return multiple intersections of the same point, unfortunately
    // We don't have tests for this yet.
}
