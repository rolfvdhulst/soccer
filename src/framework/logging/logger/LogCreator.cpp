//
// Created by rolf on 11-04-20.
//
#include <QDateTime>
#include <core/Time.h>
#include <QtCore/QDir>

#include "LogCreator.h"
bool LogCreator::isLogging() const {
    return writingLog;
}
void LogCreator::startLogging() {
    if(writingLog){
        writer.close();
    }
    QDir directory = QDir::current();
    while(directory.dirName() != "soccer"){
        bool success = directory.cdUp();
        if(!success){
            std::cerr<<"Could not find application root folder, did you rename or are you not running from root?"<<std::endl;
            std::cerr<<"Could not open log file"<<std::endl;
            return;
        }
    }
    if(!directory.cd("logfiles")){
        std::cout<<"Could not find logfiles directory, creating new directory"<<std::endl;
        directory.mkdir("logfiles");
        directory.cd("logfiles");
    }
    QString pathString = directory.absolutePath();
    QString timeString = QDateTime::currentDateTime().toString("/yyyy-MM-dd_hh-mm-ss-zzz.log");
    QString totalString = pathString+timeString;
    std::cout<<totalString.toStdString()<<std::endl;
    writingLog = writer.open(totalString);
    while (!writingLog){
        std::cerr<<"Failed to open new log file: "<<totalString.toStdString()<<", trying again"<<std::endl;
        writingLog = writer.open(totalString);
    }
}
void LogCreator::endLogging() {
    if(writingLog){
        std::cout<<"Closed log file"<<std::endl;
        writer.close();
    }
    writingLog = false;
}
void LogCreator::addLogFrame(const proto::FrameLog &logFrame) {
    writer.addLogFrame(logFrame,Time::now().asNanoSeconds());
}
