//
// Created by rolf on 29-03-20.
//

#ifndef SOCCER_REFEREEFILTER_H
#define SOCCER_REFEREEFILTER_H
#include <protobuf/GameState.pb.h>
#include <protobuf/ssl_referee.pb.h>
#include <protobuf/RobotInfo.h>
#include <protobuf/Settings.pb.h>
#include <math/geometry/Vector2.h>
class RefereeFilter {
    public:
        RefereeFilter();
        proto::TeamRobotInfo getTeamRobotInfo() const;

        proto::GameState update(const proto::Settings& settings,const std::vector<proto::Referee>& refereeMessages, const proto::World& world);
        bool flipHasChanged() const;
        void setOurTeamName(std::string name);
        bool receivedFirstMessage() const;
    private:
        void updateNoMessages(proto::GameState &newGameState, const proto::Settings& settings, const proto::World &world);
        void updateWithMessage(proto::GameState &newGameState, const proto::Settings& settings, const proto::Referee& lastRefMessage, const proto::World &world);
        proto::GameState lastGameState;
        int lastGameEventCount = 0;
        bool flipChanged = false;
        std::string ourName = "RoboTeam Twente";
        bool receivedMessage = false;
        bool inferOurColor(const proto::Settings& settings, const proto::Referee& refereeMessage);

        bool isInCommandSwitch;
        bool ballMovedInSwitch;
        proto::Referee_Command lastCommand;
        std::optional<Vector2> ballPosStartSwitch;

        proto::TeamSettings makeTeamSettings(const proto::Settings &settings,
                const proto::Referee &lastRefMessage);
        void updateRefereeInfo(proto::GameState &newGameState, const proto::Referee &lastRefMessage);
        void updateBallMoved(const proto::World &world);
        void updateFirstBallPos(const proto::World &world);
        void updateSwitchInfo(const std::optional<proto::RefereeState_Command> &switchCommand);
        void updateCommandInfo(proto::GameState &newGameState, const proto::World &world);
        void updateCommandInfo(proto::GameState &newGameState, const proto::Referee &lastRefMessage,
                const proto::World &world);
};

#endif //SOCCER_REFEREEFILTER_H
