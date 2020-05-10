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
  int i = 0;
  while(true){
    i++;
    if(i==500){
      proto::GameEvent event;
      event.set_type(proto::GameEventType::PREPARED);
      proto::GameEvent_Prepared prepared;
      prepared.set_time_taken(2.0);
      event.mutable_prepared()->CopyFrom(prepared);
      socket->sendEvent(event);
    }
    auto before = Time::now();
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