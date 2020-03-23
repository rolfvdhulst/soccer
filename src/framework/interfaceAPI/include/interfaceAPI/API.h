//
// Created by rolf on 23-03-20.
//

#ifndef SOCCER_API_H
#define SOCCER_API_H

#include <protobuf/World.pb.h>
class API {
    public:
        static API * instance();
        void setWorldState(proto::World world);
        proto::World getWorldState();
    private:
        API();
        static API * staticInstance;
        proto::World worldState;
        std::mutex worldStateMutex;
};

#endif //SOCCER_API_H
