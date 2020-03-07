//
// Created by rolf on 07-03-20.
//

#include <gtest/gtest.h>
#include <world/RobotID.h>
TEST(RobotIDTest,constructor){
    RobotID id;
    EXPECT_FALSE(id.isValid());
    id.setID(8);
    EXPECT_TRUE(id.isValid());
    EXPECT_EQ(id,8);
    EXPECT_EQ((unsigned int) id,8);
    EXPECT_EQ((uint8) id,8);

}
TEST(RobotIDTest,bounds){
    for (uint8 i = 0; i < 255; ++ i) {
        RobotID id(i);
        if (i>ID_MAX){
            EXPECT_FALSE(id.isValid());
        } else{
            EXPECT_TRUE(id.isValid());
        }
    }
}
TEST(RobotIDTest,equalities){
    RobotID two(2), three(3),secondtwo(2);
    EXPECT_TRUE(two == secondtwo);
    EXPECT_TRUE(two == 2);
    EXPECT_FALSE(two == 3);
    EXPECT_FALSE(two == three);

    EXPECT_FALSE(two != secondtwo);
    EXPECT_FALSE(two != 2);
    EXPECT_TRUE(two != 3);
    EXPECT_TRUE(two != three);

    EXPECT_TRUE(two < three);
    EXPECT_TRUE(two < 3);
    EXPECT_FALSE(two < secondtwo);
    EXPECT_FALSE(two < 2);

    EXPECT_TRUE(three > two);
    EXPECT_TRUE(three> 2);
    EXPECT_FALSE(two > three);
    EXPECT_FALSE(two > 2);
}