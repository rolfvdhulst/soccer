//
// Created by rolf on 08-05-20.
//

#ifndef SOCCER_SRC_FRAMEWORK_AI_AIBASE_H_
#define SOCCER_SRC_FRAMEWORK_AI_AIBASE_H_

#include <protobuf/World.pb.h>
#include <protobuf/GameState.pb.h>
#include <protobuf/RobotInfo.h>
#include <protobuf/messages_robocup_ssl_geometry.pb.h>
#include <optional>
#include <protobuf/RobotCommand.pb.h>
#include <protobuf/DebugInfo.pb.h>
#include <protobuf/AISettings.pb.h>

/**
 * An interface for an AI implementation to communicate with the rest of the system.
 * Any AI implementation should comply to the format.
 */
struct AITickInfo{
  proto::RobotCommands commands;
  proto::DebugAI debugInfo;
};
class AIBase {
 public:
  virtual AITickInfo tick(proto::World world, proto::GameState gameState, proto::TeamRobotInfo teamRobotInfo,
      std::optional<proto::SSL_GeometryData> geometry, proto::AISettings settings);
};

#endif //SOCCER_SRC_FRAMEWORK_AI_AIBASE_H_
