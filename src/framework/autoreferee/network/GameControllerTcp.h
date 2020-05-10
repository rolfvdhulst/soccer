//
// Created by rolf on 09-05-20.
//

#ifndef SOCCER_SRC_FRAMEWORK_AUTOREFEREE_NETWORK_GAMECONTROLLERTCP_H_
#define SOCCER_SRC_FRAMEWORK_AUTOREFEREE_NETWORK_GAMECONTROLLERTCP_H_
#include <QTcpSocket>
#include <QThread>
#include <QtNetwork/QHostAddress>
#include <protobuf/ssl_game_controller_common.pb.h>
#include <protobuf/ssl_game_controller_autoref.pb.h>

#include <mutex>
#include <queue>

class GameControllerTCP{
 public:
  GameControllerTCP();
  ~GameControllerTCP();
  bool isConnected() const;
  bool connectTcp(const QHostAddress& address);
  bool disconnectTCP();
  bool registerAutoref();
  bool receiveReply(proto::ControllerToAutoRef * reply, int waitMsDuration = 0);
  void sendEvent(const proto::GameEvent& event);
  bool sendMessage();
  bool hasMessagesToSend();
  void runCycle();
 private:
  const std::string NAME = "RoboTeam Autoref";
  std::mutex eventMutex;
  std::queue<proto::GameEvent> eventsToSend;
  bool expectingReply = false;

  QByteArray partialPacket;
  int nextPackageSize = -1;
  unsigned int sizeBytesPosition = 0;
  QHostAddress host = QHostAddress("127.0.0.1");
  QTcpSocket * socket = nullptr;
};

#endif //SOCCER_SRC_FRAMEWORK_AUTOREFEREE_NETWORK_GAMECONTROLLERTCP_H_
