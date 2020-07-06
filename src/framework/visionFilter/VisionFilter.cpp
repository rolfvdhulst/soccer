//
// Created by rolf on 22-03-20.
//

#include "VisionFilter.h"
proto::World VisionFilter::process(const std::vector<proto::SSL_WrapperPacket>& packets, Time time) {
    //We process geometry first since it contains information on the cameras
    processGeometry(packets);
    processDetections(packets);
    return worldFilter.getWorld(time);//TODO: extrapolate based on robotCommands
}
proto::World VisionFilter::process(const std::vector<proto::SSL_WrapperPacket>& packets,  const proto::TeamRobotInfo& robotInfo) {
    //We process geometry first since it contains information on the cameras
    processGeometry(packets);
    processDetections(packets);
    return worldFilter.getWorld(lastPacketTime);//TODO: extrapolate based on robotCommands
}
void VisionFilter::processDetections(const std::vector<proto::SSL_WrapperPacket> &packets) {
    if(hasNewGeometry()){
        worldFilter.updateCameras(geomFilter.getGeometry());
    }
    for (const auto& packet : packets) {
        if(packet.has_detection()){
            worldFilter.addFrame(packet.detection());
            Time detectionTime(packet.detection().t_capture());
            if (detectionTime> lastPacketTime){
                lastPacketTime = detectionTime;
            }
        }
    }
}
void VisionFilter::processGeometry(const std::vector<proto::SSL_WrapperPacket> &packets) {
    geometryUpdated = false;
    for (const auto& packet : packets){
        if(packet.has_geometry()){
            //We check if the geometry is not duplicate with an old one.
            bool newGeometry = geomFilter.process(packet.geometry());
            if (newGeometry){
                geometryUpdated = true;
            }
        }
    }
}

bool VisionFilter::hasNewGeometry() const {
    return geometryUpdated;
}
bool VisionFilter::receivedFirstGeometry() const {
    return geomFilter.receivedFirstGeometry();
}
const proto::SSL_GeometryData &VisionFilter::getGeometry() {
    return geomFilter.getGeometry();
}