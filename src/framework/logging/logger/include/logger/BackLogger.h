//
// Created by rolf on 16-04-20.
//

#ifndef SOCCER_BACKLOGGER_H
#define SOCCER_BACKLOGGER_H

#include <queue>
#include <core/Time.h>
#include "LogCreator.h"
/**
 * This class does not yet support concurrency.
 */
class BackLogger {
    public:
        void addLogFrame(const proto::FrameLog& frame);
        void removeOldFrames();
        void saveBacklog();
    private:
        const int MAX_QUEUE_LENGTH = 10000;//upper limit to ensure no overflows/memory problems
        const Time LOG_TIME = Time(30.0);// record the last 30 seconds
        std::deque<std::pair<Time,proto::FrameLog>> lastSecondsLog; //std::queue is just a wrapper of std::queue. We need to use deque for easier iteration
        LogCreator logCreator;
};

#endif //SOCCER_BACKLOGGER_H
