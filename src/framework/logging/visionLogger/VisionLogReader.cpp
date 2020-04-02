//
// Created by rolf on 01-04-20.
//

#include "VisionLogReader.h"
#include <fstream>
#include <QtGui/QtGui>
#include "VisionLogHeader.h"



bool VisionLogReader::open(const QString& fileName) {
    QByteArray fileNameBytes = fileName.toUtf8();
    const char * fname = fileNameBytes.data();
    inStream = new std::ifstream(fname, std::ios_base::in | std::ios_base::binary);
    if(!inStream->is_open()){
        errorMessage = "Error opening log file \"" + fileName +"\"!";
        return false;
    }
    FileHeader fileHeader;
    inStream->read((char*) &fileHeader, sizeof(fileHeader));
    fileHeader.version = qFromBigEndian(fileHeader.version); //The log files are in bigEndian format.
    if(strncmp(fileHeader.name,DEFAULT_FILE_HEADER_NAME,sizeof(fileHeader.name))!= 0){
        errorMessage = "Unrecognized logfile header";
        return false;
    }
    errorMessage = "Opened " + fileName + " as log version "+QString::number(fileHeader.version)+ " (default is 1)";
    return indexFile();
}
bool VisionLogReader::indexFile() {
    long startPosition = inStream->tellg();
    DataHeader dataHeader;
    index.clear();
    int packetNumber = 0; //Upper limit is ~4.2 billion. This is more than sufficient.
    while(!inStream->eof() && inStream->is_open()){
        index[packetNumber] = inStream->tellg();
        inStream->read((char *)& dataHeader, sizeof(dataHeader));
        if(inStream->bad()){
            errorMessage = "Error indexing file";
            return false;
        }
        if(inStream->eof()){
            break; //note that we have also indexed the very end of the file, so our index is exactly one longer than
            //the amount of messages
        }
        //again data is big-endian
        dataHeader.timestamp = qFromBigEndian((long long int)dataHeader.timestamp);
        dataHeader.messageType = (MessageType) qFromBigEndian((int)dataHeader.messageType);
        dataHeader.messageSize = qFromBigEndian(dataHeader.messageSize);

        inStream->seekg(long(inStream->tellg())+dataHeader.messageSize);
        packetNumber++;
    }
    inStream->clear();
    inStream->seekg(startPosition);
    return true;
}
void VisionLogReader::close() {
    inStream->close();
    delete inStream;
}
QString VisionLogReader::message() const {
    return errorMessage;
}
unsigned long VisionLogReader::fileMessageCount() {
    return index.size()-1;//-1 because we also index the very end.
}
std::pair<long long int, MessageType> VisionLogReader::nextVisionPacket(QByteArray &data) {
    return readPacket(inStream->tellg(),data);
}
std::pair<long long int, MessageType> VisionLogReader::readPacket(long offset, QByteArray &data) {
    DataHeader dataHeader;
    inStream->seekg(offset);
    if(!inStream->eof()){
        inStream->read((char*) &dataHeader, sizeof(dataHeader));
        //very last data point is invalid at eof
        if(inStream->eof()){
            inStream->clear();
            return std::make_pair(-1,MessageType::MESSAGE_INVALID);
        }
        //Convert endian
        dataHeader.timestamp = qFromBigEndian((long long int) dataHeader.timestamp);
        dataHeader.messageType = (MessageType) qFromBigEndian((int) dataHeader.messageType);
        dataHeader.messageSize = qFromBigEndian(dataHeader.messageSize);
        //read protobuf file data.
        char buffer[dataHeader.messageSize];
        inStream->read(buffer, dataHeader.messageSize);
        data = QByteArray(buffer,dataHeader.messageSize);//set data
        return std::make_pair(dataHeader.timestamp,dataHeader.messageType);
    }
    inStream->clear();
    return std::make_pair(-1,MessageType::MESSAGE_INVALID);
}
std::pair<long long int, MessageType> VisionLogReader::visionPacketAt(int packet, QByteArray &data) {
    return readPacket(index[packet],data); //TODO: maybe check if index[packet] exists because otherwise it might default to 0
}
