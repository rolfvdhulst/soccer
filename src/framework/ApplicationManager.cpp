//
// Created by rolf on 09-03-20.
//

#include "ApplicationManager.h"

void ApplicationManager::init() { setupNetworking(); }
void ApplicationManager::run() {
    while (true) {
        handleVisionPackets();
        handleRefereePackets();
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
    proto::SSL_WrapperPacket visionPacket;
    while (visionReceiver->receive(visionPacket)) {
        if (visionPacket.has_geometry() && visionPacket.geometry().has_field()){
            std::cout<<visionPacket.geometry().calib_size()<<std::endl;
            for (const auto& cam : visionPacket.geometry().calib()) {
                cam.PrintDebugString();
                std::string string = cam.SerializeAsString();
                std::cout<<string<<std::endl;
                std::cout<<std::endl;
            }
        }
//        if(visionPacket.has_detection()&&visionPacket.detection().camera_id()==1){
//            auto x=visionPacket.detection().robots_blue();
//            for (const auto& robot : x){
//                robot.PrintDebugString();
//            }
//        }
    }
}
void ApplicationManager::handleRefereePackets() {
    proto::SSL_Referee refereePacket;
    while (refereeReceiver->receive(refereePacket)) {
    }
}
