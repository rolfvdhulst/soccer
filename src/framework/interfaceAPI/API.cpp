//
// Created by rolf on 23-03-20.
//

#include "API.h"

#include <utility>
API* API::staticInstance = nullptr;

API* API::instance() {
    if (!staticInstance){
        staticInstance = new API();
    }
    return staticInstance;
}
API::API()= default;

bool API::hasCompletedFirstTick() {
    return completedFirstTick;
}
void API::setTicked() {
    completedFirstTick = true;
}
void API::addData(const proto::FrameLog &frame) {
    std::lock_guard<std::mutex> lock(frameMutex);
    frames.emplace_back(frame);
}
std::vector<proto::FrameLog> API::getData() {
    std::lock_guard<std::mutex> lock(frameMutex);
    std::vector<proto::FrameLog> returnedFrames=std::move(frames);
    frames.clear();
    return returnedFrames;
}
