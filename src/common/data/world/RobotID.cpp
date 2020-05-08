//
// Created by rolf on 23-2-20.
//

#include <string>
#include "RobotID.h"
RobotID::RobotID(uint8_t id) { bound(id); }
RobotID::operator uint8_t() const { return _id; }
bool RobotID::operator==(uint8_t id) const { return _id == id; }
bool RobotID::operator==(RobotID id) const { return _id == id._id; }
void RobotID::setID(uint8_t id) { bound(id); }
bool RobotID::isValid() const { return _id != INVALID_ID; }
bool RobotID::operator!=(RobotID id) const { return _id != id._id; }
bool RobotID::operator!=(uint8_t id) const { return _id != id; }
bool RobotID::operator<(RobotID id) const { return _id < id._id; }
bool RobotID::operator<(uint8_t id) const { return _id < id; }
bool RobotID::operator>(RobotID id) const { return _id > id._id; }
bool RobotID::operator>(uint8_t id) const { return _id > id; }
void RobotID::bound(uint8_t id) { _id = id <= ID_MAX ? id : INVALID_ID; }
RobotID::RobotID() : _id(INVALID_ID) {}
RobotID::operator unsigned int() const { return _id; }
std::string RobotID::toString() const {
    return std::to_string(_id);
}
