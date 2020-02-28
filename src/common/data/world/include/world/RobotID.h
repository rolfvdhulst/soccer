//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_ROBOTID_H
#define SOCCER_ROBOTID_H
#include <climits>
const unsigned int INVALID_ID = INT_MAX;
const unsigned int ID_MAX = 15;
class RobotID {
   public:
    RobotID();
    RobotID(unsigned int id);

    void setID(RobotID id);
    void setID(unsigned int id);
    bool operator==(RobotID id);
    bool operator==(unsigned int id);
    bool operator!=(RobotID id);
    bool operator!=(unsigned int id);
    bool isValid();
    explicit operator unsigned int() const;

   private:
    void bound(unsigned int id);
    unsigned int _id;
};

#endif  // SOCCER_ROBOTID_H
