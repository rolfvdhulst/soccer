//
// Created by rolf on 23-03-20.
//

#include "API.h"

#include <utility>
API* API::staticInstance = nullptr;

void API::setWorldState(proto::World world) {
    std::lock_guard<std::mutex> lock(worldStateMutex);
    worldState = std::move(world);
}
proto::World API::getWorldState() {
    std::lock_guard<std::mutex> lock(worldStateMutex);
    return worldState;
}
API* API::instance() {
    if (!staticInstance){
        staticInstance = new API();
    }
    return staticInstance;
}
API::API() :
worldState(proto::World()){

}
