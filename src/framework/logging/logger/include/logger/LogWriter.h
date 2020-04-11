//
// Created by rolf on 10-04-20.
//

#ifndef SOCCER_LOGWRITER_H
#define SOCCER_LOGWRITER_H

#include <QString>
#include <iostream>
#include <protobuf/FrameLog.pb.h>

class LogWriter {
    public:
        bool open(QString &file);
        void close();
        bool addLogFrame(const proto::FrameLog &logFrame,   long long int timestamp);
    private:
        std::ofstream* outStream;
};

#endif //SOCCER_LOGWRITER_H
