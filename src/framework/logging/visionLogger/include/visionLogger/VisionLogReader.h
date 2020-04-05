//
// Created by rolf on 01-04-20.
//

#ifndef SOCCER_VISIONLOGREADER_H
#define SOCCER_VISIONLOGREADER_H

#include <QString>
#include <iostream>
#include <map>
#include "MessageType.h"

class VisionLogReader {
    public:
        //VisionLogReader();
        //~VisionLogReader();
        bool open(const QString& fileName);
        void close();
        unsigned long fileMessageCount();
        /**
         * @param data
         * @return int signifying time, Message
         */
        std::pair<long long int, MessageType> nextVisionPacket(QByteArray &data);
        std::pair<long long int, MessageType> visionPacketAt(int packet,QByteArray &data);
    private:
        std::pair<long long int, MessageType> readPacket(long offset,QByteArray &data);
        bool indexFile();
        std::ifstream* inStream;
        std::map<int,long> index; //keeps track of all the file positions.
        // Alternative is to load the entire file into memory
};

#endif //SOCCER_VISIONLOGREADER_H
