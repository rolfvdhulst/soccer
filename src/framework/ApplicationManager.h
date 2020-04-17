//
// Created by rolf on 09-03-20.
//

#ifndef SOCCER_APPLICATIONMANAGER_H
#define SOCCER_APPLICATIONMANAGER_H

#include <net/sslClient/robocup_ssl_client.h>
#include <visionFilter/VisionFilter.h>
#include <memory>
#include <future>
#include <refereeFilter/RefereeFilter.h>

#include <logger/LogCreator.h>
#include <logger/BackLogger.h>

class RoboCupSSLClient;

class ApplicationManager {
   public:
    void init();
    void run(bool &exit);
    ~ApplicationManager();
   private:
    void setupNetworking();
    std::unique_ptr<RoboCupSSLClient> visionReceiver = nullptr;
    std::unique_ptr<RoboCupSSLClient> refereeReceiver = nullptr;
    VisionFilter visionFilter;
    RefereeFilter gameStateFilter;
    std::vector<proto::SSL_WrapperPacket> visionPackets;
    std::vector<proto::Referee> refereePackets;
    void receiveVision();
    void receiveReferee();
    LogCreator logger;
    BackLogger backLogger;
    int lastSavedBacklognumber = 0;
};

#endif  // SOCCER_APPLICATIONMANAGER_H
