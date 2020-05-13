//
// Created by rolf on 09-05-20.
//

#include "AutoReferee.h"
#include "network/GameControllerTcp.h"
#include <core/Time.h>
#include <thread>
void AutoReferee::init() {
  setupNetworking();
  socket = new GameControllerTCP();
  while(!socket->isConnected()){
    socket->runCycle();
  }

}
void AutoReferee::run() {
  bool val = true;
  while(val){
    auto before = Time::now();
    receiveReferee();
    receiveVision();
    proto::TeamRobotInfo defaultInfo;
    defaultInfo.mutable_blue()->CopyFrom(DEFAULT_ROBOT());
    defaultInfo.mutable_yellow()->CopyFrom(DEFAULT_ROBOT());
    visionFilter.process(visionPackets,defaultInfo);

    std::optional<proto::SSL_GeometryData> geometry;

    if(visionFilter.hasNewGeometry()){
      geometry = visionFilter.getGeometry();
    }
    if(visionFilter.receivedFirstGeometry()){

    }
    socket->runCycle();
    auto after = Time::now();
    //std::cout<<(after-before).asMilliSeconds()<<" ms"<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

}
void AutoReferee::setupNetworking() {
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
void AutoReferee::receiveReferee(){

  proto::Referee refereePacket;
  while (refereeReceiver->receive(refereePacket)) {
    refereePackets.push_back(refereePacket);
  }
  sort(refereePackets.begin(), refereePackets.end(),
       [](const proto::Referee& a,const proto::Referee& b)
       {return a.command_timestamp()<b.command_timestamp();});
}
void AutoReferee::receiveVision() {
  proto::SSL_WrapperPacket visionPacket;
  while (visionReceiver->receive(visionPacket)) {
    visionPackets.push_back(visionPacket);
  }
}