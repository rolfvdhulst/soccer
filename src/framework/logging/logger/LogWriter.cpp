//
// Created by rolf on 10-04-20.
//

#include "LogWriter.h"
#include "LogHeader.h"
#include <fstream>

bool LogWriter::open(QString &file) {
    QByteArray fileNameBytes = file.toUtf8();
    const char *fileName = fileNameBytes.constData();
    outStream = new std::ofstream(fileName,std::ios_base::out | std::ios_base::binary);
    if(!outStream->is_open()){
        std::cerr<<"Error opening log file \""<< fileName<<"\"!"<<std::endl;
        return false;
    }else{
        std::cout<<"Writing to log file \"" << fileName <<"\"!"<<std::endl;
    }
    LogFileHeader fileHeader;
    fileHeader.version = 0; // 0 is the default for now
    strncpy(fileHeader.name, DEFAULT_LOGFILE_HEADER_NAME, sizeof(fileHeader.name));
    outStream->write((char *) &fileHeader, sizeof(fileHeader));
    return true;
}
void LogWriter::close() {
    outStream->clear();
    outStream->close();
    delete outStream;
}
bool LogWriter::addLogFrame(const proto::FrameLog &logFrame,  long long int timestamp) {
    QByteArray data;
    data.resize(logFrame.ByteSizeLong());
    if(!logFrame.IsInitialized()){
        std::cerr<<"Cannot write uninitialized Log Frame to logfile! Did you set all required fields?"<<std::endl;
        return false;
    }
    if(!logFrame.SerializeToArray(data.data(),data.size())){
        std::cerr<<"Failed to serialize logFrame"<<std::endl;
        return false;
    }

    LogDataHeader dataHeader{};
    dataHeader.timestamp = timestamp; //Timestamp should be in nanoseconds
    dataHeader.messageSize = data.size();

    //Write header to file, followed by data
    outStream->write((char *) &dataHeader,sizeof(dataHeader));
    outStream->write(data.data(), data.size());
    return true;
}
