//
// Created by rolf on 18-03-20.
//

#include <field/CameraMap.h>
#include <gtest/gtest.h>
#include "ProtoExamples.h"

TEST(CameraMap, all) {
    CameraMap map;
    ASSERT_FALSE(map.hasCamera(0));
    map.addCamera(Camera(RoboCup2019Cam0()));
    EXPECT_TRUE(map.hasCamera(0));
    EXPECT_EQ((*map.getCamera(0)).worldPos(), Camera(RoboCup2019Cam0()).worldPos());
    EXPECT_EQ((*map[0]).worldPos(), Camera(RoboCup2019Cam0()).worldPos());
    EXPECT_EQ(map.getCamera(1), std::nullopt);
    EXPECT_FALSE(map.hasCamera(1));
    map.clear();
    EXPECT_FALSE(map.hasCamera(0));
}