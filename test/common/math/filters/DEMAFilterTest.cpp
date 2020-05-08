//
// Created by rolf on 20-04-20.
//

#include <math/filters/DEMAFilter.h>
#include <gtest/gtest.h>

TEST(DEMAFilter, basic){
  double initialVal = 0.0;
  double alpha = 0.5;
  DEMAFilter test(alpha,initialVal);
  EXPECT_DOUBLE_EQ(test.getState(),initialVal);
  EXPECT_DOUBLE_EQ(test.getAlpha(),alpha);
  double newAlpha = 0.8;
  test.setAlpha(newAlpha);
  EXPECT_DOUBLE_EQ(test.getAlpha(),newAlpha);
  EXPECT_DOUBLE_EQ(test.update(1.0),1-newAlpha);
  EXPECT_DOUBLE_EQ(test.update(1.0),(1-newAlpha)*(1 + newAlpha));
}

TEST(DEMAFilter, twoDim){
  Vector2 initialVal(0,0);
  double alpha = 0.5;
  DEMAFilter2D test(alpha,initialVal);
  EXPECT_DOUBLE_EQ(test.getState().x(),initialVal.x());
  EXPECT_DOUBLE_EQ(test.getState().y(),initialVal.y());
  EXPECT_DOUBLE_EQ(test.getAlpha(),alpha);
  double newAlpha = 0.8;
  test.setAlpha(newAlpha);
  EXPECT_DOUBLE_EQ(test.getAlpha(),newAlpha);
  Vector2 dir(2,1);
  EXPECT_EQ(test.update(dir),dir*(1-newAlpha));
  EXPECT_EQ(test.update(dir),dir*(1-newAlpha)*(1 + newAlpha));
}