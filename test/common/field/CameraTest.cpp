//
// Created by rolf on 16-03-20.
//

#include <gtest/gtest.h>
#include <field/Camera.h>
#include "ProtoExamples.h"

TEST(Camera, basics) {
    std::vector<proto::SSL_GeometryCameraCalibration> testData = {RoboCup2019Cam0(), RoboCup2019Cam1()};
    for (const auto &camera : testData) {
        Camera cam(camera);
        Vector3 worldPos = cam.worldPos();
        EXPECT_DOUBLE_EQ(worldPos.x(), camera.derived_camera_world_tx()/1000.0);
        EXPECT_DOUBLE_EQ(worldPos.y(), camera.derived_camera_world_ty()/1000.0);
        EXPECT_DOUBLE_EQ(worldPos.z(), camera.derived_camera_world_tz()/1000.0);
        EXPECT_EQ(cam.getID(), camera.camera_id());
        Quaternion rotation = cam.worldToCamRotation();
        //The following will not be exactly equal in general.
        //As the quaternion we receive is not exactly normalized.
        //We use doubles in our system but this input data is in floats so we need to use EXPECT_FLOAT_EQ as there are errors of
        //Order 10^-8 or smaller.
        float quatNorm = sqrtf(
                camera.q0()*camera.q0() + camera.q1()*camera.q1() + camera.q2()*camera.q2() + camera.q3()*camera.q3());
        EXPECT_FLOAT_EQ(rotation.x(), camera.q0()/quatNorm);
        EXPECT_FLOAT_EQ(rotation.y(), camera.q1()/quatNorm);
        EXPECT_FLOAT_EQ(rotation.z(), camera.q2()/quatNorm);
        EXPECT_FLOAT_EQ(rotation.w(), camera.q3()/quatNorm);
    }
}
TEST(Camera, projections) {
    std::vector<std::pair<proto::SSL_GeometryCameraCalibration, proto::SSL_DetectionRobot>> testData = {
            {RoboCup2019Cam0(), cam0Detection()}, {RoboCup2019Cam1(), cam1Detection()}};
    for (const auto &data : testData) {
        Camera cam(data.first);
        auto robot=data.second;
        Vector2Eigen imagePos(robot.pixel_x(),robot.pixel_y());
        Vector3 pos = cam.imageToField(imagePos,robot.height());
        EXPECT_NEAR(pos.x(),robot.x(),1e-3); 
        EXPECT_NEAR(pos.y(),robot.y(),1e-3);
        EXPECT_FLOAT_EQ(pos.z(),robot.height());

        Vector3 detectionPos(robot.x(),robot.y(),robot.height());
        //The robocup 2019 cameras were imperfectly calibrated due to no negative distortion support
        // so there is some error in the inverse model, but it should be minor (less than 10 pixels on a 4 k camera)
        Vector2Eigen inverseImagePos=cam.fieldToImage(detectionPos);
        EXPECT_NEAR(inverseImagePos.x(),robot.pixel_x(),10);
        EXPECT_NEAR(inverseImagePos.y(),robot.pixel_y(),10);
    }

}