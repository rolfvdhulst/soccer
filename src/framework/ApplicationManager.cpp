//
// Created by rolf on 09-03-20.
//

#include "ApplicationManager.h"
#include <interfaceAPI/API.h>
void ApplicationManager::init() {
    setupNetworking();
}
void ApplicationManager::run(bool &exit) {
    while (!exit) {
        std::vector<proto::SSL_WrapperPacket> packets;
        proto::SSL_WrapperPacket visionPacket;
        while (visionReceiver->receive(visionPacket)) {
            packets.push_back(visionPacket);
        }
        proto::World worldState = visionFilter.process(packets);
        std::string string = worldState.SerializeAsString();
        std::cout<< "app update"<<std::endl;
        std::cout<< packets.size()<<" packets"<<std::endl;
        if (string != worldString){
            std::cout<< "new app world"<<std::endl;
            worldString = string;
        }
        std::cout<<"_________"<<std::endl;
        API::instance()->setWorldState(worldState);

        handleRefereePackets();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}
void ApplicationManager::setupNetworking() {
    constexpr int DEFAULT_VISION_PORT = 10006;
    string DEFAULT_VISION_ADDRESS = "224.5.23.2";
    visionReceiver = std::make_unique<RoboCupSSLClient>(DEFAULT_VISION_PORT, DEFAULT_VISION_ADDRESS);

    constexpr int DEFAULT_REFEREE_PORT = 10003;
    string DEFAULT_REFEREE_ADDRESS = "224.5.23.1";
    refereeReceiver = std::make_unique<RoboCupSSLClient>(DEFAULT_REFEREE_PORT, DEFAULT_REFEREE_ADDRESS);

    // TODO: perhaps add loop checking if everything succeeded?
    visionReceiver->open(false);  // boolean for blocking
    refereeReceiver->open(false);
}
void ApplicationManager::handleVisionPackets() {

}
void ApplicationManager::handleRefereePackets() {
    proto::Referee refereePacket;
    while (refereeReceiver->receive(refereePacket)) {
    }
}
