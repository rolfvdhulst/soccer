//
// Created by rolf on 01-04-20.
//

#ifndef SOCCER_MESSAGETYPE_H
#define SOCCER_MESSAGETYPE_H

enum class MessageType: int
{
        MESSAGE_INVALID = -1,
        MESSAGE_BLANK = 0,
        MESSAGE_UNKNOWN = 1,
        MESSAGE_SSL_VISION_2010 = 2,
        MESSAGE_SSL_REFBOX_2013 = 3,
        MESSAGE_SSL_VISION_2014 = 4
};
#endif //SOCCER_MESSAGETYPE_H
