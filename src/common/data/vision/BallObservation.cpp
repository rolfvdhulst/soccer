//
// Created by rolf on 22-03-20.
//

#include "BallObservation.h"

#include <utility>

BallObservation::BallObservation(int cameraID, Time timeCaptured, Time timeSent,
        const proto::SSL_DetectionBall& detectionBall) :
        cameraID(cameraID),
        timeCaptured(timeCaptured),
        timeSent(timeSent),
        position(detectionBall.x()/1000.0,detectionBall.y()/1000.0), //Position by SSL-Vision is given in millimeters
        pixelPosition(detectionBall.pixel_x(),detectionBall.pixel_y()),
        confidence(detectionBall.confidence()),
        area(detectionBall.area()),
        height(detectionBall.z()/1000.0){
}

BallObservation::BallObservation(int cameraID, Time timeCaptured, Time timeSent, Eigen::Vector2d position,
                                 Eigen::Vector2d pixelPosition, double confidence, unsigned int area, double height) :
                                 cameraID(cameraID),
                                 timeCaptured(timeCaptured),
                                 timeSent(timeSent),
                                 position(std::move(position)),
                                 pixelPosition(std::move(pixelPosition)),
                                 confidence(confidence),
                                 area(area),
                                 height(height){

}
bool BallObservation::operator==(const BallObservation &other) const {
  return
  cameraID == other.cameraID &&
  timeCaptured == other.timeCaptured &&
  timeSent == other.timeSent &&
  position == other.position &&
  pixelPosition == other.pixelPosition &&
  area == other.area &&
  confidence == other.confidence &&
  height == other.height;

}
BallObservation mergeBallObservationsByArea(const std::vector<BallObservation>& observations){
    //It's the user's responsibility to ensure all observations are from the same detectionFrame
    unsigned int totalArea = 0;
    for(const auto& observation : observations){
        totalArea+=observation.area;
    }
    Eigen::Vector2d position(0,0);
    Eigen::Vector2d pixelPosition(0,0);
    double confidence = 0.0;
    double height = 0.0;
    for(const auto& observation : observations){
        double weight = observation.area/ (double) totalArea;
        position += weight * observation.position;
        pixelPosition += weight * observation.pixelPosition;
        confidence = fmax(confidence,observation.confidence);
        height += weight * observation.height;
    }
    return BallObservation(observations[0].cameraID,observations[0].timeCaptured,observations[0].timeSent,position,pixelPosition,confidence,totalArea,height);
}
