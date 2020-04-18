#include <math/geometry/Angle.h>
#include <math/geometry/Vector2.h>
#include <gtest/gtest.h>

TEST(VectorTests, instantiation) {
    Vector2 def;
    Vector2 tenten(10, 10);
    Vector2 fivezero(5, 0);
    ASSERT_DOUBLE_EQ(def.x, 0);
    ASSERT_DOUBLE_EQ(def.y, 0);
    ASSERT_DOUBLE_EQ(tenten.x, 10);
    ASSERT_DOUBLE_EQ(tenten.y, 10);
    ASSERT_DOUBLE_EQ(fivezero.x, 5);
    ASSERT_DOUBLE_EQ(fivezero.y, 0);
    Angle ang(M_PI_2);
    Vector2 vec(ang);
    ASSERT_NEAR(vec.x, 0, 1e-15);
    ASSERT_NEAR(vec.y, 1, 1e-15);
}

TEST(VectorTests, operators) {
    Vector2 def;
    Vector2 tenten(10, 10);
    Vector2 fivezero(5, 0);
    ASSERT_TRUE(def == def);
    ASSERT_TRUE(tenten == tenten);
    ASSERT_TRUE(fivezero == fivezero);
    ASSERT_FALSE(def != def);
    ASSERT_FALSE(tenten != tenten);
    ASSERT_FALSE(fivezero != fivezero);
    Vector2 tmp = def + tenten;
    ASSERT_DOUBLE_EQ(tmp.x, 10);
    ASSERT_DOUBLE_EQ(tmp.y, 10);
    tmp = tenten - fivezero;
    ASSERT_DOUBLE_EQ(tmp.x, 5);
    ASSERT_DOUBLE_EQ(tmp.y, 10);
    tmp = tenten * fivezero;
    ASSERT_DOUBLE_EQ(tmp.x, 50);
    ASSERT_DOUBLE_EQ(tmp.y, 0);
}

#define SQRT200 14.1421356237309505

TEST(VectorTests, math) {
    Vector2 def;
    Vector2 tenten(10, 10);
    Vector2 fivezero(5, 0);

    ASSERT_DOUBLE_EQ(def.length(), 0);
    ASSERT_DOUBLE_EQ(tenten.length(), SQRT200);
    ASSERT_DOUBLE_EQ(fivezero.length(), 5);

    ASSERT_DOUBLE_EQ(def.normalize().x, 0);
    ASSERT_DOUBLE_EQ(def.normalize().y, 0);
    ASSERT_DOUBLE_EQ(tenten.normalize().x, 10 / SQRT200);
    ASSERT_DOUBLE_EQ(tenten.normalize().y, 10 / SQRT200);
    ASSERT_DOUBLE_EQ(fivezero.normalize().x, 1);
    ASSERT_DOUBLE_EQ(fivezero.normalize().y, 0);

    ASSERT_DOUBLE_EQ(def.scale(3.5).x, 0);
    ASSERT_DOUBLE_EQ(def.scale(3.5).y, 0);
    ASSERT_DOUBLE_EQ(tenten.scale(3.5).x, 35);
    ASSERT_DOUBLE_EQ(tenten.scale(3.5).y, 35);
    ASSERT_DOUBLE_EQ(fivezero.scale(3.5).x, 17.5);
    ASSERT_DOUBLE_EQ(fivezero.scale(3.5).y, 0);

    ASSERT_DOUBLE_EQ(def.dist(tenten), SQRT200);
    ASSERT_DOUBLE_EQ(tenten.dist(fivezero), 11.1803398874989485);

    ASSERT_DOUBLE_EQ(def.dot(tenten), 0);
    ASSERT_DOUBLE_EQ(tenten.dot(fivezero), 50);

    // ASSERT_DOUBLE_EQ(M_PI_4, tenten.angle()); //TODO: somehow is not compiling on macOS
    ASSERT_DOUBLE_EQ(0, fivezero.angle());

    Vector2 proj = fivezero.project(def, tenten);
    ASSERT_DOUBLE_EQ(2.5, proj.x);
    ASSERT_DOUBLE_EQ(2.5, proj.y);
    proj = tenten.project(def, fivezero);
    ASSERT_DOUBLE_EQ(5, proj.x);
    ASSERT_DOUBLE_EQ(0, proj.y);
    proj = tenten.project(fivezero, def);
    EXPECT_DOUBLE_EQ(5, proj.x);
    EXPECT_DOUBLE_EQ(0, proj.y);
}

TEST(VectorTests, moreOperators) {
    Vector2 a(1, 2);
    Vector2 b(3, 4);

    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);

    EXPECT_DOUBLE_EQ((a + b).x, a.x + b.x);
    EXPECT_DOUBLE_EQ((a + b).y, a.y + b.y);
    EXPECT_DOUBLE_EQ((a - b).x, a.x - b.x);
    EXPECT_DOUBLE_EQ((a - b).y, a.y - b.y);
    EXPECT_DOUBLE_EQ((b / a).x, b.x / a.x);
    EXPECT_DOUBLE_EQ((b / a).y, b.y / a.y);

    b /= a;
    EXPECT_DOUBLE_EQ(b.x, 3);
    EXPECT_DOUBLE_EQ(b.y, 2);
    b *= a;
    EXPECT_DOUBLE_EQ(b.x, 3);
    EXPECT_DOUBLE_EQ(b.y, 4);
    Vector2 y = b * 2;
    b *= 2;
    EXPECT_DOUBLE_EQ(b.x, 6);
    EXPECT_DOUBLE_EQ(b.y, 8);
    EXPECT_DOUBLE_EQ(y.x, 6);
    EXPECT_DOUBLE_EQ(y.y, 8);
    Vector2 z = b / 2;
    b /= 2;
    EXPECT_DOUBLE_EQ(b.x, 3);
    EXPECT_DOUBLE_EQ(b.y, 4);
    EXPECT_DOUBLE_EQ(z.x, 3);
    EXPECT_DOUBLE_EQ(z.y, 4);
    b -= a;
    EXPECT_DOUBLE_EQ(b.x, 2);
    EXPECT_DOUBLE_EQ(b.y, 2);
    b -= 1;
    EXPECT_DOUBLE_EQ(b.x, 1);
    EXPECT_DOUBLE_EQ(b.y, 1);
    b += 3;
    EXPECT_DOUBLE_EQ(b.x, 4);
    EXPECT_DOUBLE_EQ(b.y, 4);
    b += a;
    EXPECT_DOUBLE_EQ(b.x, 5);
    EXPECT_DOUBLE_EQ(b.y, 6);

    Vector2 c = b + 2;
    EXPECT_DOUBLE_EQ(c.x, 7);
    EXPECT_DOUBLE_EQ(c.y, 8);
    Vector2 d = b - 1;
    EXPECT_DOUBLE_EQ(d.x, 4);
    EXPECT_DOUBLE_EQ(d.y, 5);

    Vector2 test(0, 0);
    EXPECT_TRUE(test.isNotNaN());
    double nan = std::numeric_limits<double>::quiet_NaN();
    Vector2 test2(nan, 0);
    EXPECT_FALSE(test2.isNotNaN());
    Vector2 test3(nan, nan);
    Vector2 test4(0, nan);
    EXPECT_FALSE(test3.isNotNaN());
    EXPECT_FALSE(test4.isNotNaN());
}

TEST(VectorTests, protoVector) {
    Vector2 f(3, 4);
    proto::Vector2f x = f;
    EXPECT_DOUBLE_EQ(x.x(), f.x);
    EXPECT_DOUBLE_EQ(x.y(), f.y);
    Vector2 checkF = x;
    Vector2 b = Vector2(1, 1);
    b = x;
    EXPECT_DOUBLE_EQ(checkF.x, f.x);
    EXPECT_DOUBLE_EQ(checkF.y, f.y);
    EXPECT_DOUBLE_EQ(b.x, f.x);
    EXPECT_DOUBLE_EQ(b.y, f.y);
    std::cout << checkF << std::endl;  // testing print functionality
}
TEST(VectorTests, rotate) {
    Vector2 A(sqrt(2), sqrt(2));
    Vector2 result = A.rotate(M_PI_4);
    Vector2 result2 = A.rotate(-M_PI_4);
    EXPECT_NEAR(result.x, 0, 1e-15);
    EXPECT_NEAR(result.y, 2, 1e-15);
    EXPECT_NEAR(result2.x, 2, 1e-15);
    EXPECT_NEAR(result2.y, 0, 1e-15);
}

TEST(VectorTests, lerp) {
    Vector2 x(1, 1), y(3, 3);
    Vector2 centre = x.lerp(y, 0.5);
    Vector2 centre2 = y.lerp(x, 0.5);
    Vector2 origin = x.lerp(y, 1.5);
    Vector2 origin2 = y.lerp(x, -0.5);
    EXPECT_DOUBLE_EQ(centre.x, 2);
    EXPECT_DOUBLE_EQ(centre.y, 2);
    EXPECT_DOUBLE_EQ(centre2.x, 2);
    EXPECT_DOUBLE_EQ(centre2.y, 2);

    EXPECT_DOUBLE_EQ(origin.x, 0);
    EXPECT_DOUBLE_EQ(origin.y, 0);
    EXPECT_DOUBLE_EQ(origin.x, 0);
    EXPECT_DOUBLE_EQ(origin.y, 0);
}

TEST(VectorTests, stretchToLength) {
    Vector2 zero;
    Vector2 zeroStretched = zero.stretchToLength(1.0);
    EXPECT_DOUBLE_EQ(zeroStretched.x, 1.0);
    EXPECT_DOUBLE_EQ(zeroStretched.y, 0.0);

    Vector2 usual(3, 4);
    Vector2 usualStretched = usual.stretchToLength(10);  // Twice as long
    EXPECT_DOUBLE_EQ(usualStretched.x, 6);
    EXPECT_DOUBLE_EQ(usualStretched.y, 8);
    EXPECT_DOUBLE_EQ(usualStretched.length(), 10);
    EXPECT_DOUBLE_EQ(usualStretched.angle(), usual.angle());
}

TEST(VectorTests, cross) {
    Vector2 A(1, 2), B(3, 4);
    EXPECT_DOUBLE_EQ(A.cross(B), A.x * B.y - A.y * B.x);
    EXPECT_DOUBLE_EQ(B.cross(A), B.x * A.y - B.y * A.x);
}

TEST(VectorTests, project2) {
    Vector2 direction(2, 2);
    Vector2 null(2, 0);
    Vector2 result1 = null.project2(direction);
    Vector2 result2 = direction.project2(null);
    EXPECT_DOUBLE_EQ(result1.x, 1);
    EXPECT_DOUBLE_EQ(result1.y, 1);
    EXPECT_DOUBLE_EQ(result2.x, 2);
    EXPECT_DOUBLE_EQ(result2.y, 0);
}
