//
// Created by rolf on 22-03-20.
//

#ifndef SOCCER_VISIONFILTER_H
#define SOCCER_VISIONFILTER_H
#include <protobuf/World.pb.h>
#include <protobuf/messages_robocup_ssl_wrapper.pb.h>
#include "GeometryFilter.h"
#include "WorldFilter.h"

class VisionFilter {
    public:
        proto::World process(const std::vector<proto::SSL_WrapperPacket>& packets);
        bool hasNewGeometry();
        const proto::SSL_GeometryData& getGeometry();
    private:
        bool geometryUpdated = false;
        GeometryFilter geomFilter;
        WorldFilter worldFilter;
        Time lastPacketTime;

        void processGeometry(const std::vector<proto::SSL_WrapperPacket> &packets);
        void processDetections(const std::vector<proto::SSL_WrapperPacket> &packets);
};

#endif //SOCCER_VISIONFILTER_H
