//
// Created by rolf on 02-04-20.
//

#include "VisionLogWriter.h"
#include "VisionLogHeader.h"
#include <fstream>
#include <QtEndian>

bool VisionLogWriter::open(QString &file) {
    QByteArray fileNameBytes = file.toUtf8();
    const char *fileName = fileNameBytes.constData();
    outStream = new std::ofstream(fileName,std::ios_base::out | std::ios_base::binary);
    if(!outStream->is_open()){
        std::cerr<<"Error opening log file \""<< fileName<<"\"!"<<std::endl;
        return false;
    }else{
        std::cout<<"Writing to log file \"" << fileName <<"\"!"<<std::endl;
    }
    FileHeader fileHeader;
    fileHeader.version = 1; // 1 is the default version used (0 is using old deprecated messages)
    fileHeader.version = qToBigEndian(fileHeader.version); // everything is stored big-endian
    strncpy(fileHeader.name, DEFAULT_FILE_HEADER_NAME,sizeof(fileHeader.name));
    outStream->write((char *) &fileHeader, sizeof(fileHeader));
    return true;
}
void VisionLogWriter::addVisionPacket(const proto::SSL_WrapperPacket &frame, long long time) {
    QByteArray data;
    data.resize(frame.ByteSize());
    if(!frame.IsInitialized()){
        std::cerr<<"Writing uninitialized vision packet to logfile"<<std::endl;
    }
    if(!frame.SerializeToArray(data.data(),data.size())){
        std::cerr<<"Writing to array failed for vision packet for logging!"<<std::endl;

    }
    writePacket(data,time,MessageType::MESSAGE_SSL_VISION_2014);
}
void VisionLogWriter::addRefereePacket(const proto::Referee &refState, long long int time) {
    QByteArray data;
    data.resize(refState.ByteSize());
    if(!refState.IsInitialized()){
        std::cerr<<"Writing uninitialized referee packet to logfile"<<std::endl;
    }
    if(!refState.SerializeToArray(data.data(),data.size())){
        std::cerr<<"Writing to array failed for referee packet for logging!"<<std::endl;
    }
    writePacket(data,time,MessageType::MESSAGE_SSL_REFBOX_2013);
}
void VisionLogWriter::writePacket(const QByteArray &data, long long int time, MessageType type) {
    DataHeader dataHeader;
    dataHeader.timestamp = time;
    dataHeader.messageType = type;
    dataHeader.messageSize = data.size();

    dataHeader.timestamp = qToBigEndian((long long int) dataHeader.timestamp);
    dataHeader.messageType = (MessageType) (qToBigEndian((int)dataHeader.messageType));
    dataHeader.messageSize = qToBigEndian(dataHeader.messageSize);

    outStream->write((char *) &dataHeader,sizeof(dataHeader));

    outStream->write(data.constData(), data.size());
}
void VisionLogWriter::close() {
    outStream->clear();
    outStream->close();
    delete outStream;
}

