//
// Created by rolf on 23-03-20.
//

#ifndef SOCCER_API_H
#define SOCCER_API_H

#include <protobuf/FrameLog.pb.h>
#include <mutex>

class API {
    public:
        static API * instance();

        void addData(const proto::FrameLog &frame);
        std::vector<proto::FrameLog> getData();
        bool hasCompletedFirstTick();
        void setTicked();
    private:
        API();
        static API * staticInstance;

        std::mutex frameMutex;
        std::vector<proto::FrameLog> frames;

        bool completedFirstTick = false;
};

#endif //SOCCER_API_H
