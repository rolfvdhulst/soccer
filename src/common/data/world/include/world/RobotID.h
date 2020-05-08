//
// Created by rolf on 23-2-20.
//
#ifndef SOCCER_ROBOTID_H
#define SOCCER_ROBOTID_H
#include <cstdint>

const uint8_t INVALID_ID = 255;
const uint8_t ID_MAX = 15;
//TODO: move to different library than world?
class RobotID {
   public:
    RobotID();
    RobotID(uint8_t id);

    void setID(uint8_t id);
    bool operator==(RobotID id) const;
    bool operator==(uint8_t id) const;
    bool operator!=(RobotID id) const;
    bool operator!=(uint8_t id) const;
    // We implement the following so sorting algorithms etc.can be used
    bool operator<(RobotID id) const;
    bool operator<(uint8_t id) const;
    bool operator>(RobotID id) const;
    bool operator>(uint8_t id) const;
    [[nodiscard]] bool isValid() const;
    explicit operator unsigned int() const;
    explicit operator uint8_t() const;
    [[nodiscard]] std::string toString() const;

   private:
    void bound(uint8_t id);
    unsigned int _id;
};

#endif  // SOCCER_ROBOTID_H
