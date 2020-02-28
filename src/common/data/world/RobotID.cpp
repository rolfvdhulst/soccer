//
// Created by rolf on 23-2-20.
//

#include "RobotID.h"
RobotID::RobotID(unsigned int id){
    bound(id);
}
RobotID::operator unsigned int() const {
    return _id;
}
bool RobotID::operator==(unsigned int id) {
    return _id == id;
}
bool RobotID::operator==(RobotID id) {
    return _id == id._id;
}
void RobotID::setID(unsigned int id) {
    bound(id);
}
void RobotID::setID(RobotID id) {
    bound(id._id);
}
bool RobotID::isValid() {
    return _id != INVALID_ID;
}
bool RobotID::operator!=(RobotID id) {
    return _id != id._id;
}
bool RobotID::operator!=(unsigned int id) {
    return _id != id;
}
void RobotID::bound(unsigned int id) {
    _id = id<=ID_MAX ? id : INVALID_ID;
}
RobotID::RobotID() : _id(INVALID_ID){


}
