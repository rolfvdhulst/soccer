//
// Created by rolf on 10-04-20.
//

#ifndef SOCCER_LOGREADER_H
#define SOCCER_LOGREADER_H
#include <QString>
#include <iostream>
#include <protobuf/FrameLog.pb.h>

class LogReader {
    public:
        bool open(const QString& fileName);
        void close();
        unsigned int fileMessageCount();
        long long int getNextTime();
        /**
         * @param data
         * @return int representing time in nanoseconds, the logging frame
         */
        std::pair<long long int, proto::FrameLog > nextFrame();
        std::pair<long long int, proto::FrameLog > frameAt(unsigned int packet);
        void resetToStartOfFile();
    private:
        std::pair<long long int, proto::FrameLog > readPacket(long offset);
        bool indexFile();
        std::ifstream* inStream;
        std::map<int,long> index; //keeps track of all the file positions.
        long startPosition; //index before very first message
        // Alternatives are to load the entire file into memory (which can become problematically large), or to read as you go along
};

#endif //SOCCER_LOGREADER_H
