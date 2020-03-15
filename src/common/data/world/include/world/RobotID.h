//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_ROBOTID_H
#define SOCCER_ROBOTID_H
#include <tiff.h>

const uint8 INVALID_ID = 255;
const uint8 ID_MAX = 15;

class RobotID {
   public:
    RobotID();
    RobotID(uint8 id);

    void setID(uint8 id);
    bool operator==(RobotID id) const;
    bool operator==(uint8 id) const;
    bool operator!=(RobotID id) const;
    bool operator!=(uint8 id) const;
    // We implement the following so sorting algorithms etc.can be used
    bool operator<(RobotID id) const;
    bool operator<(uint8 id) const;
    bool operator>(RobotID id) const;
    bool operator>(uint8 id) const;
    [[nodiscard]] bool isValid() const;
    explicit operator unsigned int() const;
    explicit operator uint8() const;

   private:
    void bound(uint8 id);
    unsigned int _id;
};

#endif  // SOCCER_ROBOTID_H
