//
// Created by rolf on 29-03-20.
//

#ifndef SOCCER_REFEREEFILTER_H
#define SOCCER_REFEREEFILTER_H
#include <protobuf/GameState.pb.h>
#include <protobuf/ssl_referee.pb.h>
class RefereeFilter {
    public:
        proto::GameState update(const std::vector<proto::Referee>& refereeMessages, const proto::World& world);
        bool flipHasChanged() const;
    private:
        proto::GameState lastGameState;
        int lastGameEventCount = 0;
        bool flipChanged = false;
        static bool inferOurColor(const proto::Referee& refereeMessage);
        proto::GameState createGameState(const proto::Referee &lastRefMessage,bool weAreBlue);
        void addCommands(proto::Referee& newRefMessage,bool weAreBlue, const proto::World& world);
};

#endif //SOCCER_REFEREEFILTER_H
