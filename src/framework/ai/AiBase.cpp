//
// Created by rolf on 08-05-20.
//

#include "AiBase.h"
AITickInfo AIBase::tick(proto::World world,
                        proto::GameState gameState,
                        proto::TeamRobotInfo teamRobotInfo,
                        std::optional<proto::SSL_GeometryData> geometry,
                        proto::AISettings settings) {
  return AITickInfo();
}
