//
// Created by rolf on 22-03-20.
//

#include "VisionFilter.h"
proto::World VisionFilter::process(const std::vector<proto::SSL_WrapperPacket>& packets) {
    bool timeUpdated = false;

    for (const auto& packet : packets){
        //First always send geometry packets to geometryFilter
        if(packet.has_geometry()){
            geomFilter.process(packet.geometry());
        }
        if(packet.has_detection()){
            worldFilter.addFrame(packet.detection());
            Time detectionTime(packet.detection().t_capture());
            if (detectionTime>lastPacketTime){
                lastPacketTime = detectionTime;
                timeUpdated = true;
            }
        }
    }

    if (!timeUpdated && !packets.empty()){
        std::cout<<"something fishy.."<<std::endl;
    }
    return worldFilter.getWorld(lastPacketTime);//TODO: extrapolate based on robotCommands
}
const FieldState& VisionFilter::getField() {
    return geomFilter.getField();
}
