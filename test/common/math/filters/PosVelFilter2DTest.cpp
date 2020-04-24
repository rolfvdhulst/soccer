//
// Created by rolf on 20-04-20.
//

#include <core/Random.h>
#include <gtest/gtest.h>
#include <math/filters/PosVelFilter2D.h>
TEST(PosVelFilter2D, constantPosition) {
  int numSamples = 100;

  Random gen(0);

  Eigen::Vector4d initialState = Eigen::Vector4d::Zero();
  PosVelFilter2D filter(initialState, Eigen::Matrix4d::Identity(), 0.1, 0.01, Time());
  for (int i = 0; i < numSamples; i++) {
    Time time(0.01*i);
    double posX = gen.getGaussian()*0.02; //2 cm noise
    double posY = gen.getGaussian()*0.02; //
    filter.predict(time);
    filter.update({posX,posY});
  }
  EXPECT_NEAR(filter.getVelocity().x(),0,0.01); // less than 1 cm per second
  EXPECT_NEAR(filter.getVelocity().y(),0,0.01);
  EXPECT_NEAR(filter.getPosition().x(),0,0.04); //within 4 cm of actual position (twice the variance == 95 %...)
  EXPECT_NEAR(filter.getPosition().y(),0,0.04);
  EXPECT_NEAR(filter.getPositionUncertainty().x(),0,0.04); //within 4 cm of actual position
  EXPECT_NEAR(filter.getPositionUncertainty().y(),0,0.04);
  EXPECT_NEAR(filter.getVelocityUncertainty().x(),0,0.1); //within .1 m/s
  EXPECT_NEAR(filter.getVelocityUncertainty().y(),0,0.1); //within .1 m/s
}

TEST(PosVelFilter2D, constantVelocity) {
  int numSamples = 101;
  double velocity = 5.0;
  Random gen(0);

  Eigen::Vector4d initialState = Eigen::Vector4d::Zero();
  PosVelFilter2D filter(initialState, Eigen::Matrix4d::Identity(), 0.1, 0.01, Time());
  for (int i = 0; i < numSamples; i++) {
    Time time(0.01*i);
    double posX = time.asSeconds()*velocity + gen.getGaussian()*0.02; //2 cm noise
    double posY = time.asSeconds()*velocity +gen.getGaussian()*0.02; //
    filter.predict(time);
    filter.update({posX,posY});
  }
  EXPECT_NEAR(filter.getVelocity().x(),velocity,0.01*velocity); //within 5% of actual velocity
  EXPECT_NEAR(filter.getVelocity().y(),velocity,0.01*velocity);
  EXPECT_NEAR(filter.getPosition().x(),velocity,0.005); //within .5 cm of actual position
  EXPECT_NEAR(filter.getPosition().y(),velocity,0.005);
  EXPECT_NEAR(filter.getPositionUncertainty().x(),0,0.05); //within 5 cm of actual position
  EXPECT_NEAR(filter.getPositionUncertainty().y(),0,0.05);
  EXPECT_NEAR(filter.getVelocityUncertainty().x(),0,0.1); //within .1 m/s
  EXPECT_NEAR(filter.getVelocityUncertainty().y(),0,0.1); //within .1 m/s

  filter.setMeasurementError(1e-8);
  filter.predict(Time(1.01));
  filter.update({5.05,5.05});
  EXPECT_NEAR(filter.getVelocity().x(),velocity,0.01*velocity); //within 5% of actual velocity
  EXPECT_NEAR(filter.getVelocity().y(),velocity,0.01*velocity);

  auto state =filter.getState();
  EXPECT_DOUBLE_EQ(state[0],filter.getPosition().x());
  EXPECT_DOUBLE_EQ(state[1],filter.getPosition().y());
  EXPECT_DOUBLE_EQ(state[2],filter.getVelocity().x());
  EXPECT_DOUBLE_EQ(state[3],filter.getVelocity().y());

  filter.setState(Eigen::Vector4d::Identity());
  EXPECT_DOUBLE_EQ(filter.getState()[0],1.0);
  EXPECT_DOUBLE_EQ(filter.getState()[2],0.0);
  filter.setVelocity(Eigen::Vector2d::Identity());
  EXPECT_DOUBLE_EQ(filter.getState()[2],1.0);
  filter.setPosition(3*Eigen::Vector2d::Identity());
  EXPECT_DOUBLE_EQ(filter.getState()[0],3.0);

  EXPECT_DOUBLE_EQ(filter.getPositionEstimate(Time(0.0))[0],3.0); //from before
  EXPECT_DOUBLE_EQ(filter.getPositionEstimate(Time(1.01))[0],3.0); //Right now
  EXPECT_DOUBLE_EQ(filter.getPositionEstimate(Time(2.01))[0],4.0); //3.0 +1.0 m/s * 1 second

  EXPECT_NE(filter.getPositionUncertainty()[0],1.0);
  filter.setCovariance(Eigen::Matrix4d::Identity());
  EXPECT_DOUBLE_EQ(filter.getPositionUncertainty()[0],1.0);
  EXPECT_THROW(filter.predict(Time(-1.0)),std::invalid_argument);

}
TEST(PosVelFilter2D, missingUpdates) {
    int numSamples = 100;
    Random gen(0);

    Eigen::Vector4d initialState = Eigen::Vector4d::Zero();

    PosVelFilter2D filter(initialState, Eigen::Matrix4d::Identity(), 0.1, 2, Time());
    for (int i = 0; i < numSamples; i++) {
        long time = (long)(i * 0.01 * 1e9);
        double posX = gen.getGaussian();
        double posY = gen.getGaussian();
        Eigen::Vector2d lastUnc = filter.getPositionUncertainty();
        // vision blackout between i 50 and i 70
        if ((i > 50) && (i < 70)) {
            filter.predict(Time(time));
            EXPECT_GE(filter.getPositionUncertainty().x(),lastUnc.x());
            EXPECT_GE(filter.getPositionUncertainty().y(),lastUnc.y());
        } else if (i == 70) {
            filter.predict(Time(time));
            filter.update({posX, posY});
            EXPECT_LE(filter.getPositionUncertainty().x(),lastUnc.x());
            EXPECT_LE(filter.getPositionUncertainty().y(),lastUnc.y());
        } else {
            filter.predict(Time(time));
            filter.update({posX, posY});
        }
    }
}