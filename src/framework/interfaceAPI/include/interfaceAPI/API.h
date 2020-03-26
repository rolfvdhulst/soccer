//
// Created by rolf on 23-03-20.
//

#ifndef SOCCER_API_H
#define SOCCER_API_H

#include <protobuf/World.pb.h>
#include <protobuf/messages_robocup_ssl_wrapper.pb.h>
#include <mutex>

class API {
    public:
        static API * instance();
        void setWorldState(const proto::World &world);
        proto::World getWorldState();
        void addDetectionFrames(const std::vector<proto::SSL_WrapperPacket> &frames);
        std::vector<proto::SSL_WrapperPacket> getFramesAndClear();
        void addGeometryData(const proto::SSL_GeometryData& data);
        proto::SSL_GeometryData readGeometryData();
        bool newGeometry();

    private:
        API();
        static API * staticInstance;
        proto::World worldState;
        std::mutex worldStateMutex;

        std::vector<proto::SSL_WrapperPacket> detectionFrames;
        std::mutex detectionFramesMutex;

        proto::SSL_GeometryData geometryData;
        bool geometryWasUpdated = false;

        std::mutex geometryMutex;
};

#endif //SOCCER_API_H
