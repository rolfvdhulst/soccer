//
// Created by rolf on 23-2-20.
//

#include "RobotID.h"
RobotID::RobotID(uint8 id) { bound(id); }
RobotID::operator uint8() const { return _id; }
bool RobotID::operator==(uint8 id) const{ return _id == id; }
bool RobotID::operator==(RobotID id) const{ return _id == id._id; }
void RobotID::setID(uint8 id) { bound(id); }
bool RobotID::isValid() const{ return _id != INVALID_ID; }
bool RobotID::operator!=(RobotID id) const { return _id != id._id; }
bool RobotID::operator!=(uint8 id) const { return _id != id; }
bool RobotID::operator<(RobotID id) const { return _id < id._id; }
bool RobotID::operator<(uint8 id) const { return _id < id; }
bool RobotID::operator>(RobotID id) const { return _id > id._id; }
bool RobotID::operator>(uint8 id) const { return _id > id; }
void RobotID::bound(uint8 id) { _id = id <= ID_MAX ? id : INVALID_ID; }
RobotID::RobotID() : _id(INVALID_ID) {}
RobotID::operator unsigned int() const { return _id; }
