//
// Created by rolf on 09-05-20.
//

#include "GameControllerTcp.h"
#include <iostream>
#include <thread>
#include "google/protobuf/util/delimited_message_util.h"

bool GameControllerTCP::isConnected() const {
  return socket->state() == QAbstractSocket::ConnectedState;
}
bool GameControllerTCP::receiveReply(proto::ControllerToAutoRef *reply, int waitMsDuration) {
  socket->waitForReadyRead(waitMsDuration);
  QByteArray data = socket->readAll();
  if(data.size() == 0){
    return false;
  }
  partialPacket.append(data);
  if (nextPackageSize < 0 && partialPacket.size() > 0) {
    while (sizeBytesPosition < static_cast<unsigned int>(partialPacket.size())) {
      if ((partialPacket[sizeBytesPosition] & 0x80) == 0) {
        const uint8_t *buffer = reinterpret_cast<unsigned char*>(data.data());
        google::protobuf::io::CodedInputStream varIntReader(buffer, sizeBytesPosition + 1);
        unsigned int packageSize = 0;
        varIntReader.ReadVarint32(&packageSize);
        nextPackageSize = static_cast<int>(packageSize);
        partialPacket.remove(0, sizeBytesPosition + 1);
        sizeBytesPosition = 0;
        break;
      } else {
        sizeBytesPosition++;
      }
    }
  }
  if (nextPackageSize >= 0 && partialPacket.size() >= nextPackageSize) {
    bool parseSuccess = reply->ParseFromArray(partialPacket.data(), nextPackageSize);
    partialPacket.remove(0, nextPackageSize);
    nextPackageSize = -1;
    return parseSuccess;
  }
  return false;

}

bool GameControllerTCP::connectTcp(const QHostAddress& address) {
  socket->connectToHost(address,10007);
  bool connected = socket->waitForConnected(1000);
  if(connected){
    std::cout<<"Connected to Game-controller on IP: "<< address.toString().toStdString()<<std::endl;
  }else{
    std::cerr<<"Failed to connect to Game-Controller: "<<socket->errorString().toStdString()<<std::endl;
  }
  return connected;
}
bool GameControllerTCP::disconnectTCP(){
  socket->abort();
  bool success = socket->state()== QAbstractSocket::UnconnectedState;
  if(!success){
    std::cerr<<"Failed to disconnect: "<<socket->errorString().toStdString()<<std::endl;
  }else{
    std::cout<<"Disconnected with Game-Controller"<<std::endl;
  }
  return success;
}
GameControllerTCP::GameControllerTCP() {
  socket = new QTcpSocket();
  socket->setSocketOption(QAbstractSocket::LowDelayOption,true);
}
GameControllerTCP::~GameControllerTCP() {
  delete socket;
}
void GameControllerTCP::sendEvent(const proto::GameEvent& event) {
  std::lock_guard<std::mutex> lock(eventMutex);
  eventsToSend.push(event);
}
bool GameControllerTCP::sendMessage() {
  std::lock_guard<std::mutex> lock(eventMutex);
  if(eventsToSend.empty()){
    return false;
  }
  proto::GameEvent event = eventsToSend.front();
  eventsToSend.pop();
  proto::AutoRefToController eventMessage;
  eventMessage.mutable_game_event()->CopyFrom(event);
  eventMessage.PrintDebugString();
  if(!eventMessage.IsInitialized()){
    return false; //Event message is invalid somehow
  }
  QByteArray data;
  data.resize(eventMessage.ByteSize()+4);
  google::protobuf::io::ArrayOutputStream arrout(data.data(),data.size());
  google::protobuf::io::CodedOutputStream output(&arrout);
  bool serialized = google::protobuf::util::SerializeDelimitedToCodedStream(eventMessage,&output);
  if(!serialized){
    return false;//Event message could not be serialized correctly
  }
  qint64 bytesSent = socket->write(data.data(),data.size());
  return bytesSent>0;
}
bool GameControllerTCP::registerAutoref() {
  proto::ControllerToAutoRef reply;
  bool success =receiveReply(&reply,100);
  reply.PrintDebugString();
  if(!success){
    return false; //Did not receive an initial message from an autoref so aborting
  }
  proto::AutoRefRegistration registration;
  registration.set_identifier(NAME);
  QByteArray data;
  data.resize(registration.ByteSize()+4);
  google::protobuf::io::ArrayOutputStream arrout(data.data(),data.size());
  google::protobuf::io::CodedOutputStream output(&arrout);
  bool serialized =google::protobuf::util::SerializeDelimitedToCodedStream(registration,&output);
  if(!serialized){
    return false;//registration message was not serialized correctly
  }
  qint64 bytesSent = socket->write(data.data(),data.size());
  if(bytesSent==0){
    return false;//Could not send message correctly
  }
  proto::ControllerToAutoRef confirmation;
  bool confirmationReplyReceived = receiveReply(&confirmation,100);
  if(!confirmationReplyReceived){
    std::cerr<<"Did not receive confirmation of registration"<<std::endl;
    return false;
  }
  if(confirmation.controller_reply().status_code() == proto::ControllerReply_StatusCode::ControllerReply_StatusCode_OK){
    std::cout<<"Registered successfully!"<<std::endl;
    return true;
  }else{
    std::cerr<<"Unsuccessfull registration: "<< confirmation.controller_reply().reason()<<std::endl;
    return false;
  }
}
void GameControllerTCP::runCycle() {
  if(!isConnected()){
    if(!(disconnectTCP()&&connectTcp(QHostAddress("127.0.0.1"))&&registerAutoref())){
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));//wait for a second before retrying to reconnect and register
      return;
    } else{
      expectingReply = false;
    }
  }
  if(expectingReply){
    proto::ControllerToAutoRef reply;
    bool receivedReply = receiveReply(&reply);
    if(receivedReply){
      expectingReply = false;
      if(reply.controller_reply().status_code() != proto::ControllerReply_StatusCode_OK){
        std::cerr<<"ControllerReply status bad: "<<reply.controller_reply().reason()<<std::endl;
      }
      reply.PrintDebugString();
    }

  } else{
    bool sentMessage = sendMessage();
    if(sentMessage){
      expectingReply = true;
    }
  }
}
bool GameControllerTCP::hasMessagesToSend() {
  return !eventsToSend.empty();
}

