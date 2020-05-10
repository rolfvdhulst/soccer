//

#ifndef SOCCER_SRC_FRAMEWORK_AUTOREFEREE_AUTOREFEREE_H_
#define SOCCER_SRC_FRAMEWORK_AUTOREFEREE_AUTOREFEREE_H_
#include <net/sslClient/robocup_ssl_client.h>
#include <visionFilter/VisionFilter.h>
#include <memory>
#include <future>
#include <refereeFilter/RefereeFilter.h>

class RoboCupSSLClient;
class GameControllerTCP;
class AutoReferee {
 public:
  void init();
  void run();
 private:
  void setupNetworking();
  GameControllerTCP * socket;
  std::unique_ptr<RoboCupSSLClient> visionReceiver = nullptr;
  std::unique_ptr<RoboCupSSLClient> refereeReceiver = nullptr;
  VisionFilter visionFilter;
  RefereeFilter gameStateFilter;
  std::vector<proto::SSL_WrapperPacket> visionPackets;
  std::vector<proto::Referee> refereePackets;
  void receiveVision();
  void receiveReferee();
};

#endif //SOCCER_SRC_FRAMEWORK_AUTOREFEREE_AUTOREFEREE_H_
