//
// Created by rolf on 11-04-20.
//

#ifndef SOCCER_LOGCREATOR_H
#define SOCCER_LOGCREATOR_H

#include "LogWriter.h"
class LogCreator {
    public:
        void startLogging();
        bool isLogging() const;
        void addLogFrame(const proto::FrameLog &logFrame);
        void endLogging();
    private:
        LogWriter writer;
        bool writingLog = false;
};

#endif //SOCCER_LOGCREATOR_H
