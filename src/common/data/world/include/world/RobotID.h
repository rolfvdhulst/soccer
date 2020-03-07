//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_ROBOTID_H
#define SOCCER_ROBOTID_H
#include <tiff.h>
#include <climits>
const uint8 INVALID_ID = 255;
const uint8 ID_MAX = 15;
class RobotID {
   public:
    RobotID();
    RobotID(uint8 id);

    void setID(RobotID id);
    void setID(uint8 id);
    bool operator==(RobotID id);
    bool operator==(uint8 id);
    bool operator!=(RobotID id);
    bool operator!=(uint8 id);
    bool isValid();
    explicit operator unsigned int() const;
    explicit operator uint8() const;

   private:
    void bound(uint8 id);
    unsigned int _id;
};

#endif  // SOCCER_ROBOTID_H
