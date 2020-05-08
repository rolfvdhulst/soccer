//
// Created by rolf on 22-04-20.
//

#include <math/filters/ConstantMAFilter.h>
#include <gtest/gtest.h>

TEST(ConstantMAFilter, basic){
  double initialVal = 0.0;
  ConstantMAFilter<3> filter;
  double result =filter.update(3.0);
  EXPECT_DOUBLE_EQ(result,3.0);
  EXPECT_DOUBLE_EQ(filter.update(0.0),1.5);
  EXPECT_DOUBLE_EQ(filter.update(0.0),1.0);
  EXPECT_DOUBLE_EQ(filter.update(0.0),0.0);
  EXPECT_DOUBLE_EQ(filter.update(-3.0),-1.0);

}

TEST(ConstantMAFilter, twoDim){
  double initialVal = 0.0;
  ConstantMAFilter2D<3> filter;
  Vector2 initial(3.0,6.0);
  Vector2 result =filter.update(initial);
  EXPECT_DOUBLE_EQ(result.x(),initial.x());
  EXPECT_DOUBLE_EQ(result.y(),initial.y());

  EXPECT_EQ(filter.usedMeasurements(),1);

  result = filter.update(Vector2());
  EXPECT_DOUBLE_EQ(result.x(),1.5);
  EXPECT_DOUBLE_EQ(result.y(),3.0);

  result = filter.update(Vector2());
  EXPECT_DOUBLE_EQ(result.x(),1.0);
  EXPECT_DOUBLE_EQ(result.y(),2.0);

  result = filter.update(Vector2());
  EXPECT_DOUBLE_EQ(result.x(),0.0);
  EXPECT_DOUBLE_EQ(result.y(),0.0);

  EXPECT_EQ(filter.usedMeasurements(),3);
  result = filter.update({-3.0,0.0});
  EXPECT_DOUBLE_EQ(result.x(),-1.0);
  EXPECT_DOUBLE_EQ(result.y(),0.0);

  EXPECT_EQ(filter.usedMeasurements(),3);
}