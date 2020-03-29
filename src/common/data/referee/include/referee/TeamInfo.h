// Created by rolf on 29-03-20.
//

#ifndef SOCCER_TEAMINFO_H
#define SOCCER_TEAMINFO_H
#include <string>
#include <core/Time.h>
#include <vector>
#include <world/RobotID.h>

#include <protobuf/ssl_referee.pb.h>
class TeamInfo {

    public:
        TeamInfo(const proto::Referee::TeamInfo& teamInfo);
        [[nodiscard]] unsigned int activeYellowCards() const;
        std::string name;
        unsigned int score;
        unsigned int redCards;
        std::vector<Time> yellowCardTimes;
        unsigned int totalYellowCards;
        unsigned int timeOuts;
        Time timeOutTime;
        RobotID goalkeeperID;
        //Below fields might not be set, we default values in that case
        unsigned int foulCounter = 0;
        unsigned int ballPlacementFailures = 0;
        bool canPlaceBall = true;
        unsigned int maxAllowedBots; //Default initialized based on yellow and redCards
        bool botSubstitutionIntent = false;
        bool ballPlacementFailuresReached = false;
};

#endif //SOCCER_TEAMINFO_H
