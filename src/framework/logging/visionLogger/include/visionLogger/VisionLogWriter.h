//
// Created by rolf on 02-04-20.
//

#ifndef SOCCER_VISIONLOGWRITER_H
#define SOCCER_VISIONLOGWRITER_H

#include <QString>
#include <iostream>

#include <protobuf/messages_robocup_ssl_wrapper.pb.h>
#include <protobuf/ssl_referee.pb.h>
#include "MessageType.h"

class VisionLogWriter {
    public:
        //VisionLogWriter();
        //~VisionLogWriter();
        bool open(QString &file);
        void close();
        void addVisionPacket(const proto::SSL_WrapperPacket &frame, long long int time);
        void addRefereePacket(const proto::Referee& refState, long long int time);
    private:
        void writePacket(const QByteArray &data,long long int time ,MessageType type);

        std::ofstream* outStream;
};

#endif //SOCCER_VISIONLOGWRITER_H
