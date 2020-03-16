//
// Created by rolf on 16-03-20.
//

#ifndef SOCCER_PROTOEXAMPLES_H
#define SOCCER_PROTOEXAMPLES_H
#include <protobuf/messages_robocup_ssl_geometry.pb.h>
#include <protobuf/messages_robocup_ssl_detection.pb.h>
//** camera helpers
proto::SSL_GeometryCameraCalibration RoboCup2019Cam0(){
    proto::SSL_GeometryCameraCalibration camera;
    camera.set_camera_id(0);
    camera.set_derived_camera_world_tx(-4540.39502);
    camera.set_derived_camera_world_ty(834.644409);
    camera.set_derived_camera_world_tz(6152.91211);

    camera.set_tx(-837.344788);
    camera.set_ty(4451.72217);
    camera.set_tz(6217);

    camera.set_q0(-0.713628769);
    camera.set_q1(-0.700438142);
    camera.set_q2(-0.0109084724);
    camera.set_q3(0.00101082819);

    camera.set_focal_length(1453.91357);
    camera.set_distortion(-0.00999999978);

    camera.set_principal_point_x(1443.72058);
    camera.set_principal_point_y(717.867004);
    return camera;
}
proto::SSL_DetectionRobot cam0Detection(){
    proto::SSL_DetectionRobot robot;
    robot.set_confidence(0.95226711);
    robot.set_robot_id(2);
    robot.set_x(-5153.92236);
    robot.set_y(-3257.67725);
    robot.set_orientation(-0.000495584507);
    robot.set_pixel_x(412.477875);
    robot.set_pixel_y(561.0177);
    robot.set_height(150);
    return robot;
}
proto::SSL_GeometryCameraCalibration RoboCup2019Cam1(){
    proto::SSL_GeometryCameraCalibration camera;
    camera.set_camera_id(1);
    camera.set_derived_camera_world_tx(3800.79883);
    camera.set_derived_camera_world_ty(-759.607117);
    camera.set_derived_camera_world_tz(6048.93213);

    camera.set_tx(-838.942078);
    camera.set_ty(3717.62769);
    camera.set_tz(6090);

    camera.set_q0(0.700048983);
    camera.set_q1(-0.713936985);
    camera.set_q2(0.0031630001);
    camera.set_q3(-0.01468);

    camera.set_focal_length(1361.73291);
    camera.set_distortion(-0.00328200008);

    camera.set_principal_point_x(1411.21851);
    camera.set_principal_point_y(818.82782);
    return camera;
}
proto::SSL_DetectionRobot cam1Detection(){
    proto::SSL_DetectionRobot robot;
    robot.set_confidence(0.95221746);
    robot.set_robot_id(6);
    robot.set_x(3617.12256);
    robot.set_y(-150.607895);
    robot.set_orientation(0.0205682535);
    robot.set_pixel_x(1236.57727);
    robot.set_pixel_y(842.203247);
    robot.set_height(147);
    return robot;
}

#endif //SOCCER_PROTOEXAMPLES_H
