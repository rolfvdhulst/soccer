//
// Created by rolf on 07-03-20.
//

#include <math/geometry/Rotation.h>
#include <gtest/gtest.h>
TEST(RotationTests, basics) {
    Rotation a;
    EXPECT_DOUBLE_EQ(a.getRotation(), 0.0);
    Rotation b(5.0);
    Rotation c(b);
    EXPECT_DOUBLE_EQ(b.getRotation(), c.getRotation());
    c.setRotation(2.0);
    EXPECT_DOUBLE_EQ(c.getRotation(), 2.0);
    EXPECT_DOUBLE_EQ((double)b, 5.0);
}
TEST(RotationTests, equalities) {
    Rotation checkOne(1.0);
    Rotation smallDif(1.0 + 1e-7);
    Rotation other(2.0);
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
}
TEST(RotationTests, plusMinus) {
    Rotation a(2.0), b(2.5), c(2.0);
    Rotation d(-2.0);

    Rotation e = d - a;
    EXPECT_DOUBLE_EQ(e.getRotation(), -4.0);
    e = d - 2.0;
    EXPECT_DOUBLE_EQ(e.getRotation(), -4.0);
    e = b + a;
    EXPECT_DOUBLE_EQ(e.getRotation(), 4.5);
    e = b + 2.0;
    EXPECT_DOUBLE_EQ(e.getRotation(), 4.5);

    e = c += a;
    EXPECT_DOUBLE_EQ(e.getRotation(), 4.0);
    EXPECT_DOUBLE_EQ(c.getRotation(), 4.0);
    e = c += (-1.0);
    EXPECT_DOUBLE_EQ(e.getRotation(), 3);
    EXPECT_DOUBLE_EQ(c.getRotation(), 3);

    e = d -= b;
    EXPECT_DOUBLE_EQ(e.getRotation(), -4.5);
    EXPECT_DOUBLE_EQ(d.getRotation(), -4.5);
    e = d -= (-2.0);
    EXPECT_DOUBLE_EQ(e.getRotation(), -2.5);
    EXPECT_DOUBLE_EQ(d.getRotation(), -2.5);
}