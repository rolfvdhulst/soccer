//
// Created by rolf on 09-03-20.
//

#include "ApplicationManager.h"
#include <interfaceAPI/API.h>
#include <protoUtils/Flip.h>
#include <protobuf/FrameLog.pb.h>
#include <interfaceAPI/SettingsAPI.h>

void ApplicationManager::init() {
    setupNetworking();
    //we wait for the first time we receive information from the interface (it has some startup time)
    while(!SettingsAPI::instance()->receivedFirstSettings()){

    }
}
void ApplicationManager::run(bool &exit) {
    int count = 0;
    Time total((long)0);
    while (!exit) {
        proto::Settings settings = SettingsAPI::instance()->getSettings();
        bool newReplay = false;
        if(settings.loggingon() && !logger.isLogging()){
            logger.startLogging();
            newReplay = true;
        } else if(!settings.loggingon() && logger.isLogging()){
            logger.endLogging();
        }
        if(!settings.playingreplay()) {
            proto::FrameLog log;
            Time before = Time::now();
            receiveVision();
            receiveReferee();

            proto::TeamRobotInfo teamRobotInfo = gameStateFilter.getTeamRobotInfo();

            proto::World worldState = visionFilter.process(visionPackets, teamRobotInfo);
            proto::GameState gameState = gameStateFilter.update(settings, refereePackets, worldState);

            std::optional<proto::SSL_GeometryData> geometryData;
            //We resend the geometry if new geometry has arrived
            // or if we change the rotation of data
            //Or when we start a new logfile so the current known geometry is logged
            if (visionFilter.hasNewGeometry()
                    || (gameStateFilter.flipHasChanged() && visionFilter.receivedFirstGeometry())
                    || (newReplay && visionFilter.receivedFirstGeometry())
                    ){
                geometryData = visionFilter.getGeometry();
            }
            if (gameState.settings().weplayonpositivehalf()) {
                //Flip world and geometry. GameState is always already flipped the right way because it computes this value
                flip(worldState);
                if (geometryData) {
                    flip(*geometryData);
                }

            }
            //For now we also flip the detection packets like this. Later if we really want to re-run using logging we might want to NOT do this for saving logs
            // This makes logging in the interface a bit easier
            std::vector<proto::SSL_WrapperPacket> copy = visionPackets;
            for (auto &visionPacket : copy) {
                if (visionPacket.has_detection() && gameState.settings().weplayonpositivehalf()) {
                    flip(visionPacket.mutable_detection());
                }
                //TODO: also flip raw geometry
                log.add_visionmessages()->CopyFrom(visionPacket);
            }
            for (const auto &refPacket : refereePackets) {
                log.add_refereemessages()->CopyFrom(refPacket);
            }
            log.mutable_robotinfo()->CopyFrom(teamRobotInfo);
            log.mutable_world()->CopyFrom(worldState);
            log.mutable_gamestate()->CopyFrom(gameState);
            log.mutable_replaysettings()->CopyFrom(settings);

            if (geometryData) {
                log.mutable_interpretedgeometry()->CopyFrom(*geometryData);
            }

            if (logger.isLogging()) {
                logger.addLogFrame(log);
            }
            backLogger.addLogFrame(log);
            backLogger.removeOldFrames();
            //This line informs the interface of EVERYTHING
            API::instance()->addData(log);
            API::instance()->setTicked();
            refereePackets.clear();
            visionPackets.clear();

            Time after = Time::now();
            total += (after - before);
            count ++;
            if (count%100 == 0) {
                std::cout << total.asSeconds()*1000/count << std::endl;
                total = Time(0.0);
                count = 0;
            }
        }
        if(settings.has_savebacklog() && settings.savebacklog() && settings.messagecounter() != lastSavedBacklognumber){
            lastSavedBacklognumber = settings.messagecounter();
            backLogger.saveBacklog();
        }
        this_thread::sleep_for(std::chrono::milliseconds(3));
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
