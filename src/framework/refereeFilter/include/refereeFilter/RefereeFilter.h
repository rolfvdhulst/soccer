//
// Created by rolf on 29-03-20.
//

#ifndef SOCCER_REFEREEFILTER_H
#define SOCCER_REFEREEFILTER_H
#include <protobuf/GameState.pb.h>
#include <protobuf/ssl_referee.pb.h>
#include <protobuf/RobotInfo.h>
#include <protobuf/Settings.pb.h>
#include <geometry/Vector2.h>
class RefereeFilter {
    public:
        proto::TeamRobotInfo getTeamRobotInfo() const;
        proto::GameState update(const proto::Settings& settings,const std::vector<proto::Referee>& refereeMessages, const proto::World& world);
        bool flipHasChanged() const;
    private:
        proto::GameState lastGameState;
        int lastGameEventCount = 0;
        bool flipChanged = false;
        static bool inferOurColor(const proto::Settings& settings, const proto::Referee& refereeMessage);
        proto::GameState createGameState(const proto::Settings& settings, const proto::Referee &lastRefMessage,bool weAreBlue,const proto::World& world);
        void addCommands(proto::GameState& gameState, const proto::Referee& lastRefMessage,bool weAreBlue, const proto::World& world);
        bool isInCommandSwitch;
        bool ballMovedInSwitch;
        proto::Referee_Command lastCommand;
        std::optional<Vector2> ballPosStartSwitch;
};

#endif //SOCCER_REFEREEFILTER_H
