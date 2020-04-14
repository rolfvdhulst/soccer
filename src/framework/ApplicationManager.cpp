//
// Created by rolf on 09-03-20.
//

#include "ApplicationManager.h"
#include <interfaceAPI/API.h>
#include <geometry/Flip.h>
#include <protobuf/FrameLog.pb.h>
#include <interfaceAPI/SettingsAPI.h>

void ApplicationManager::init() {
    setupNetworking();
    while(!SettingsAPI::instance()->receivedFirstSettings()){

    }
}
void ApplicationManager::run(bool &exit) {
    int count = 0;
    Time total((long)0);
    while (!exit) {
        proto::Settings settings = SettingsAPI::instance()->getSettings();
        if(settings.loggingon() && !logger.isLogging()){
            logger.startLogging();
        } else if(!settings.loggingon() && logger.isLogging()){
            logger.endLogging();
        }
        proto::FrameLog log;
        Time before= Time::now();
        receiveVision();
        receiveReferee();
        for (const auto& visionPacket : visionPackets ) {
            log.add_visionmessages()->CopyFrom(visionPacket);
        }
        for (const auto& refPacket : refereePackets){
            log.add_refereemessages()->CopyFrom(refPacket);
        }
        proto::TeamRobotInfo teamRobotInfo = gameStateFilter.getTeamRobotInfo();

        proto::World worldState = visionFilter.process(visionPackets,teamRobotInfo);
        proto::GameState gameState = gameStateFilter.update(settings,refereePackets,worldState);

        std::optional<proto::SSL_GeometryData> geometryData;
        //We resend the geometry if new geometry has arrived or if we change the rotation of data
        if (visionFilter.hasNewGeometry() || (gameStateFilter.flipHasChanged() && visionFilter.receivedFirstGeometry())){
            geometryData = visionFilter.getGeometry();
        }
        if (gameState.settings().weplayonpositivehalf()){
            //Flip world and geometry. GameState is always already flipped the right way because it computes this value
            flip(worldState);
            if(geometryData){
                flip(*geometryData);
            }
        }
        log.mutable_robotinfo()->CopyFrom(teamRobotInfo);
        log.mutable_world()->CopyFrom(worldState);
        log.mutable_gamestate()->CopyFrom(gameState);
        log.mutable_replaysettings()->CopyFrom(settings);

        if(geometryData){
            log.mutable_interpretedgeometry()->CopyFrom(*geometryData);
        }

        //Send the relevant information to the interface
        if(geometryData){
            API::instance()->addGeometryData(*geometryData);
        }
        std::vector<proto::SSL_WrapperPacket> copy = visionPackets;
        if(gameState.settings().weplayonpositivehalf()){
            for(auto& packet : copy){
                if(packet.has_detection()){
                    flip(packet.mutable_detection());
                }
            }
        }
        if(logger.isLogging()){
            logger.addLogFrame(log);
        }
        API::instance()->addDetectionFrames(copy);

        if(gameState.has_referee()){
            std::vector<proto::GameEvent> events;
            for (const auto & gameEvent : gameState.referee().game_events()) {
                events.push_back(gameEvent);
            }
            API::instance()->addGameEvents(events);
        }

        API::instance()->setWorldState(worldState);
        API::instance()->setGameState(gameState);
        API::instance()->setTicked();
        refereePackets.clear();
        visionPackets.clear();

        Time after = Time::now();
        total +=(after-before);
        count++;
        if(count%100 == 0){
            std::cout<< total.asSeconds()*1000/count<<std::endl;
            total = Time(0.0);
            count = 0;
        }
        this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
void ApplicationManager::receiveReferee(){

    proto::Referee refereePacket;
    while (refereeReceiver->receive(refereePacket)) {
       refereePackets.push_back(refereePacket);
    }
    sort(refereePackets.begin(), refereePackets.end(),
            [](const proto::Referee& a,const proto::Referee& b)
            {return a.command_timestamp()<b.command_timestamp();});
}
void ApplicationManager::receiveVision() {
    proto::SSL_WrapperPacket visionPacket;
    while (visionReceiver->receive(visionPacket)) {
        visionPackets.push_back(visionPacket);
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
ApplicationManager::~ApplicationManager() {
    visionReceiver->close();
    refereeReceiver->close();
}
