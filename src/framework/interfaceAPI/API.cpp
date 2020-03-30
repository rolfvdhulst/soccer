//
// Created by rolf on 23-03-20.
//

#include "API.h"

#include <utility>
API* API::staticInstance = nullptr;

void API::setWorldState(const proto::World& world) {
    std::lock_guard<std::mutex> lock(worldStateMutex);
    worldState = world;
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
worldState(proto::World()){ }
void API::addDetectionFrames(const std::vector<proto::SSL_WrapperPacket> &frames) {
    std::lock_guard<std::mutex> lock(detectionFramesMutex);
    detectionFrames.insert(detectionFrames.end(),frames.begin(),frames.end());
}
std::vector<proto::SSL_WrapperPacket> API::getFramesAndClear() {
    std::lock_guard<std::mutex> lock(detectionFramesMutex);
    std::vector<proto::SSL_WrapperPacket> frames=std::move(detectionFrames);
    detectionFrames.clear();
    return frames;
}
void API::addGeometryData(const proto::SSL_GeometryData &data) {
    std::lock_guard<std::mutex> lock(geometryMutex);
    geometryWasUpdated = true;
    geometryData = data;
}
proto::SSL_GeometryData API::readGeometryData() {
    std::lock_guard<std::mutex> lock(geometryMutex);
    geometryWasUpdated = false; //We read the geometry so no need to worry about this anymore
    return geometryData;
}
bool API::newGeometry() {
    std::lock_guard<std::mutex> lock(geometryMutex);
    return geometryWasUpdated;
}
void API::setGameState(const proto::GameState& newGameState) {
    std::lock_guard<std::mutex> lock(gameStateMutex);
    gameState = newGameState;
}
proto::GameState API::getGameState() {
    std::lock_guard<std::mutex> lock(gameStateMutex);
    return gameState;
}
void API::addGameEvents(const std::vector<proto::GameEvent> &events) {
    std::lock_guard<std::mutex> lock(gameEventsMutex);
    gameEvents.insert(gameEvents.end(),events.begin(),events.end());
}
std::vector<proto::GameEvent> API::getGameEvents() {
    std::lock_guard<std::mutex> lock(gameEventsMutex);
    std::vector<proto::GameEvent> events = std::move(gameEvents);
    gameEvents.clear();
    return events;
}