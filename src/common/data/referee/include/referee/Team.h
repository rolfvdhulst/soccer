//
// Created by rolf on 28-03-20.
//

#ifndef SOCCER_TEAM_H
#define SOCCER_TEAM_H

#include <protobuf/ssl_game_controller_common.pb.h>
class Team {
    public:
        enum Value{// team not set
                  UNKNOWN = 0,
                  // yellow team
                  YELLOW = 1,
                  // blue team
                  BLUE = 2
        };
        Team();
        explicit constexpr Team(proto::Team team);
        std::string toString() const;
        operator Value() const; //allow switches and comparisons
        explicit operator bool() = delete; //Prevent if(Team) incorrect usage
        constexpr bool operator ==(Team other) const;
        constexpr bool operator !=(Team other) const;
    private:
        Value value = UNKNOWN;

};
#endif //SOCCER_TEAM_H