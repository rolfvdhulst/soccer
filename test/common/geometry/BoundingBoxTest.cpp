//
// Created by rolf on 02-05-20.
//

#include <math/geometry/BoundingBox2D.h>
#include <gtest/gtest.h>
#include <math/geometry/Ray.h>

TEST(BoundingBoxTest,constructors){
  BoundingBox2D constructor1(-1,2,4,5);
  EXPECT_EQ(constructor1.xMin(),-1);
  EXPECT_EQ(constructor1.xMax(),4);
  EXPECT_EQ(constructor1.yMin(),2);
  EXPECT_EQ(constructor1.yMax(),5);

  BoundingBox2D constructor2(Vector2(4,5),Vector2(-1,2));
  EXPECT_EQ(constructor2.xMin(),-1);
  EXPECT_EQ(constructor2.xMax(),4);
  EXPECT_EQ(constructor2.yMin(),2);
  EXPECT_EQ(constructor2.yMax(),5);

  BoundingBox2D constructor3(Vector2(4,5));
  EXPECT_EQ(constructor3.xMin(),4);
  EXPECT_EQ(constructor3.xMax(),4);
  EXPECT_EQ(constructor3.yMin(),5);
  EXPECT_EQ(constructor3.yMax(),5);

  BoundingBox2D constructor4;
  EXPECT_EQ(constructor4.xMin(),-std::numeric_limits<double>::infinity());
  EXPECT_EQ(constructor4.xMax(),std::numeric_limits<double>::infinity());
  EXPECT_EQ(constructor4.yMin(),-std::numeric_limits<double>::infinity());
  EXPECT_EQ(constructor4.yMax(),std::numeric_limits<double>::infinity());


}
TEST(BoundingBoxTest,operatorPlus){
  BoundingBox2D a(-1,-1,1,1), b(-2,-2,2,2);

  EXPECT_EQ(a+b,b);
  EXPECT_EQ(b+a,b);

  BoundingBox2D &f= a+=b;
  EXPECT_EQ(f,b);
  EXPECT_EQ(a,b);

  BoundingBox2D c(3,3,5,5);

  EXPECT_EQ(b+c,BoundingBox2D(-2,-2,5,5));
  EXPECT_EQ(c+Vector2(4,6),BoundingBox2D(3,3,5,6));
  BoundingBox2D &g= c+=Vector2(4,6);
  EXPECT_EQ(g,BoundingBox2D(3,3,5,6));

  BoundingBox2D last(3,3,5,5);

  EXPECT_EQ(last+Vector2(6,6),BoundingBox2D(3,3,6,6));
  EXPECT_EQ(last+Vector2(2,2),BoundingBox2D(2,2,5,5));
}

TEST(BoundingBoxTest,contains){
  BoundingBox2D c(3,3,5,5);
  EXPECT_TRUE(c.contains(Vector2(4,4)));
  EXPECT_TRUE(c.contains(Vector2(3,3)));
  EXPECT_TRUE(c.contains(Vector2(3,3.5)));
  EXPECT_FALSE(c.contains(Vector2(2,4)));
  EXPECT_FALSE(c.contains(Vector2(2,2)));
}
TEST(BoundingBoxTest,overlap){
  BoundingBox2D c(3,3,5,5);
  BoundingBox2D d(4,4,6,9);
  EXPECT_TRUE(c.doesOverlap(d));
  EXPECT_EQ(*c.overlap(d),BoundingBox2D(4,4,5,5));
  BoundingBox2D e(4,6,6,9);
  EXPECT_FALSE(c.doesOverlap(e));
  EXPECT_EQ(c.overlap(e),std::nullopt);
}

TEST(BoundingBoxTest,rayIntersect){
  BoundingBox2D box(0,0,1,1);
  Ray inside(Vector2(0.5,0.5),Vector2(0.8,0.8));
  Ray insideOtherWay(Vector2(0.6,0.6),Vector2(0.4,0.4));
  Ray touchCorner(Vector2(0.5,-1),Vector2(1,0));
  for(const auto& ray : {inside,insideOtherWay,touchCorner}){
    EXPECT_TRUE(box.doesIntersect(ray));
  }

  Ray outsideAboveRising(Vector2(-1,-1),Vector2(0,3));
  Ray outsideAboveDescending(Vector2(-1,10),Vector2(0,9));
  Ray outsideBelowRising(Vector2(0.5,-1),Vector2(1,-0.5));
  Ray outsideBelowDescending(Vector2(-1,1),Vector2(0,-1));
  for(const auto& ray : {outsideAboveRising,outsideAboveDescending,outsideBelowRising,outsideBelowDescending}){
    EXPECT_FALSE(box.doesIntersect(ray));
  }
  Ray horizontalTouch(Vector2(-2,0),Vector2(-1,0));
  Ray horizontalTouch2(Vector2(0.5,0),Vector2(1.5,0));
  Ray horizontalTouch3(Vector2(0,0),Vector2(1,0));
  Ray horizontalTouch4(Vector2(1,0),Vector2(2,0));
  Ray horizontalTouchAway(Vector2(-1,0),Vector2(-2,0));;
  Ray horizontalIntersect(Vector2(-1,0.5),Vector2(-0.5,0.5));
  Ray horizontalIntersectAway(Vector2(-0.5,0.5),Vector2(-1,0.5));
  Ray horizontalOutside(Vector2(-2,-2),Vector2(-1,-2));
  Ray horizontalOutsideAway(Vector2(-1,-2),Vector2(-2,-2));
  for(const auto& ray : {horizontalTouch, horizontalIntersect, horizontalTouch2, horizontalTouch3, horizontalTouch4}) {
    EXPECT_TRUE(box.doesIntersect(ray));
  }
  for(const auto& ray : {horizontalTouchAway, horizontalIntersectAway,horizontalOutside,horizontalOutsideAway}) {
    EXPECT_FALSE(box.doesIntersect(ray));
  }
  Ray verticalTouch(Vector2(0,-2),Vector2(0,-1));
  Ray verticalTouchAway(Vector2(0,-1),Vector2(0,-2));;
  Ray verticalIntersect(Vector2(0.5,-1),Vector2(0.5,-0.5));
  Ray verticalIntersectAway(Vector2(0.5,-0.5),Vector2(0.5,-1));
  Ray verticalOutside(Vector2(-2,-2),Vector2(-2,-1));
  Ray verticalOutsideAway(Vector2(-2,-1),Vector2(-2,-2));
  for(const auto& ray : {verticalTouch, verticalIntersect}) {
    EXPECT_TRUE(box.doesIntersect(ray));
  }
  for(const auto& ray : {verticalTouchAway, verticalIntersectAway,verticalOutside,verticalOutsideAway}) {
    EXPECT_FALSE(box.doesIntersect(ray));
  }
}