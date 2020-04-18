//
// Created by rolf on 04-03-20.
//

#include <math/geometry/Angle.h>
#include <math/geometry/Vector2.h>
#include <gtest/gtest.h>

TEST(AngleTests, constructors) {
    Angle zero{};
    EXPECT_DOUBLE_EQ(zero.getAngle(), 0.0);
    Angle vertical(M_PI_2);
    EXPECT_DOUBLE_EQ(vertical.getAngle(), M_PI_2);
    Angle copy(vertical);
    EXPECT_DOUBLE_EQ(copy.getAngle(), M_PI_2);
    Angle vectorConstr(Vector2(1, 1));
    EXPECT_DOUBLE_EQ(vectorConstr.getAngle(), M_PI_4);
    Angle vectorConstr2(Vector2(1e-7, 1e-7));
    EXPECT_DOUBLE_EQ(vectorConstr2.getAngle(), 0.0);
}

TEST(AngleTests, constrain) {
    Angle leftMost(-M_PI);
    Angle fullRotation(2 * M_PI);
    Angle rightMost(M_PI);
    Angle randomRotation(2 * M_PI + 1);
    EXPECT_DOUBLE_EQ(leftMost.getAngle(), -M_PI);
    EXPECT_DOUBLE_EQ(fullRotation.getAngle(), 0);
    EXPECT_DOUBLE_EQ(rightMost.getAngle(), -M_PI);
    EXPECT_DOUBLE_EQ(randomRotation.getAngle(), 1.0);
}

TEST(AngleTests, setAngle) {
    Angle angle(1.0);
    EXPECT_DOUBLE_EQ(angle.getAngle(), 1.0);
    // Also check if constrained properly
    angle.setAngle(2 * M_PI - 1);
    EXPECT_DOUBLE_EQ(angle.getAngle(), -1.0);
}

TEST(AngleTests, toVector2) {
    Vector2 start(1.0, 0);
    Angle angle(start);
    EXPECT_DOUBLE_EQ(angle.toVector2(1.0).x, start.x);
    EXPECT_DOUBLE_EQ(angle.toVector2(1.0).y, start.y);
    Angle angle2(Vector2(1, 1));
    Vector2 test = angle2.toVector2(1.0);
    EXPECT_DOUBLE_EQ(test.x, sqrt(2) * 0.5);
    EXPECT_DOUBLE_EQ(test.y, sqrt(2) * 0.5);
}
TEST(AngleTests, copyAssignment) {
    Angle angle(1.0);
    angle = (M_PI);
    EXPECT_DOUBLE_EQ(angle.getAngle(), -M_PI);  // check if it explicitly constrains
    EXPECT_DOUBLE_EQ((double)angle, -M_PI);
}

TEST(AngleTests, equalities) {
    Angle checkOne(1.0);
    Angle smallDif(1.0 + 1e-7);
    Angle other(2.0);
    EXPECT_TRUE(checkOne == smallDif);
    EXPECT_TRUE(smallDif == checkOne);
    EXPECT_FALSE(checkOne == other);
    EXPECT_FALSE(other == checkOne);
    EXPECT_TRUE(checkOne == (1 + 1e-7));
    EXPECT_FALSE(checkOne == 2.0);

    EXPECT_FALSE(checkOne != smallDif);
    EXPECT_FALSE(smallDif != checkOne);
    EXPECT_TRUE(checkOne != other);
    EXPECT_TRUE(other != checkOne);
    EXPECT_FALSE(checkOne != (1 + 1e-7));
    EXPECT_TRUE(checkOne != 2.0);

    Angle edge(-M_PI);
    Angle otherSide(M_PI - 1e-7);
    EXPECT_TRUE(edge == otherSide);
}
TEST(AngleTests, plusMinus) {
    Angle a(2.0), b(2.5), c(2.0);
    Angle d(-2.0);

    Angle e = d - a;
    EXPECT_DOUBLE_EQ(e.getAngle(), -4.0 + 2 * M_PI);
    e = d - 2.0;
    EXPECT_DOUBLE_EQ(e.getAngle(), -4.0 + 2 * M_PI);
    e = b + a;
    EXPECT_DOUBLE_EQ(e.getAngle(), 4.5 - 2 * M_PI);
    e = b + 2.0;
    EXPECT_DOUBLE_EQ(e.getAngle(), 4.5 - 2 * M_PI);

    e = c += a;
    EXPECT_DOUBLE_EQ(e.getAngle(), 4.0 - 2 * M_PI);
    EXPECT_DOUBLE_EQ(c.getAngle(), 4.0 - 2 * M_PI);
    e = c += (-1.0);
    EXPECT_DOUBLE_EQ(e.getAngle(), 3);
    EXPECT_DOUBLE_EQ(c.getAngle(), 3);

    e = d -= b;
    EXPECT_DOUBLE_EQ(e.getAngle(), -4.5 + 2 * M_PI);
    EXPECT_DOUBLE_EQ(d.getAngle(), -4.5 + 2 * M_PI);
    e = d -= (-2.0);
    EXPECT_DOUBLE_EQ(e.getAngle(), -2.5);
    EXPECT_DOUBLE_EQ(d.getAngle(), -2.5);
}

TEST(AngleTests, angleDifference) {
    Angle a(-2.0), b(-2 + M_PI + 0.1), c(-2 + M_PI - 0.1);
    EXPECT_DOUBLE_EQ(a.angleDiff(b), M_PI - 0.1);
    // This goes the other way so other sign. Note it's still 'equally far' away
    EXPECT_DOUBLE_EQ(a.angleDiff(c), -(M_PI - 0.1));
    EXPECT_DOUBLE_EQ(a.angleDiff(-2 + M_PI + 0.1), M_PI - 0.1);
    EXPECT_DOUBLE_EQ(a.angleDiff(-2 + M_PI - 0.1), -(M_PI - 0.1));
}