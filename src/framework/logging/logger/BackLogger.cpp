//
// Created by rolf on 16-04-20.
//

#include "BackLogger.h"
void BackLogger::addLogFrame(const proto::FrameLog &frame) {
    lastSecondsLog.emplace_back(Time::now(),frame);
    while(lastSecondsLog.size() > MAX_QUEUE_LENGTH){
        lastSecondsLog.pop_front();//remove last element
    }
    //std::cout<<"Logsize: "<<lastSecondsLog.size()<<std::endl;
}
void BackLogger::removeOldFrames() {
    //If the time recorded since the logged frames is larger than the constant log time (30 seconds we pop them)
    while(!lastSecondsLog.empty() &&
    lastSecondsLog.front().first.timeSince()>LOG_TIME){
        lastSecondsLog.pop_front();
    }
}
void BackLogger::saveBacklog() {
    removeOldFrames();//run one last time before saving file to ensure it's really only 30 seconds at the moment of saving
    logCreator.startLogging();
    for (const auto& frame : lastSecondsLog ) {
        logCreator.addLogFrame(frame.second,frame.first);
    }
    logCreator.endLogging();
}
