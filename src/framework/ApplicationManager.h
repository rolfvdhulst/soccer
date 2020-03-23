//
// Created by rolf on 09-03-20.
//

#ifndef SOCCER_APPLICATIONMANAGER_H
#define SOCCER_APPLICATIONMANAGER_H

#include <net/sslClient/robocup_ssl_client.h>
#include <visionFilter/VisionFilter.h>
#include <memory>
#include <future>

class RoboCupSSLClient;

class ApplicationManager {
   public:
    void init();
    void run(bool &exit);

   private:
    void setupNetworking();
    void handleRefereePackets();
    void handleVisionPackets();
    std::unique_ptr<RoboCupSSLClient> visionReceiver = nullptr;
    std::unique_ptr<RoboCupSSLClient> refereeReceiver = nullptr;
    VisionFilter visionFilter;
    std::string worldString = " ";

};

#endif  // SOCCER_APPLICATIONMANAGER_H
