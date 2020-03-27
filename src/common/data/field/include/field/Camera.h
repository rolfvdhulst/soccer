//
// Created by rolf on 07-02-20.
//

#ifndef RTT_CAMERA_H
#define RTT_CAMERA_H

#include <geometry/Vector3.h>
#include <protobuf/messages_robocup_ssl_geometry.pb.h>
#include <Eigen/Dense>

class Camera {
   public:
    /**
     * @brief Constructs a camera from a proto message.
     * @param protoCam
     */
    explicit Camera(const proto::SSL_GeometryCameraCalibration& protoCam);
    /**
     * Gets the 3d position of the camera in !! meters !!
     * @return the 3d position where the camera is located
     */
    [[nodiscard]] Eigen::Vector3d worldPos() const;
    /**
     * @brief Gets the quaternion of the camera that represents it's 3d rotation
     * @return the 3d orientation of the camera
     */
    [[nodiscard]] Eigen::Quaterniond worldToCamRotation() const;
    /**
     * @brief Gets the camera id
     * @return the id of the camera.
     */
    [[nodiscard]] unsigned int getID() const;
    // All of the below functions are just Eigen3 implementations of the logic in SSL-Vision, so nothing here is really 'created' by us.
    // If you have problems understanding it I recommend looking further at camera_calibration.h in ssl-vision
    // or to send me a message so I can help explaining it.
    // This code might be changed in the future so it might break compatibility.
    /**
     * @brief maps 3d field points to points on the camera image coordinates
     * @param fieldPoint  3d point to be mapped to image !! in mm !!
     * @return the pixel coordinates of the 3d point that was mapped.
     */
    [[nodiscard]] Eigen::Vector2d fieldToImage(const Eigen::Vector3d& fieldPoint) const;
    /**
     * !!RETURNS position in millimeter!!
     * @param imagePoint point in image that is used to determine object position
     * @param assumedHeight height that object is assumed to be detected at
     * @return 3d Position of the point in world coordinates (in mm!)
     */
    [[nodiscard]] Eigen::Vector3d imageToField(const Eigen::Vector2d& imagePoint, double assumedHeight) const;

   private:
    [[nodiscard]] Eigen::Vector2d radialDistortion(Eigen::Vector2d& imagePoint) const;
    [[nodiscard]] double radialDistortion(double radius) const;
    [[nodiscard]] double radialDistortionInv(double radius) const;
    [[nodiscard]] Eigen::Vector2d radialDistortionInv(Eigen::Vector2d& imagePoint) const;

    unsigned int id;
    double focalLength;
    Vector2Eigen principalPoint;
    Vector3 position;
    Vector3 translation;
    Quaternion orientation;
    double distortion;
};

#endif  // RTT_CAMERA_H
