//
// Created by rolf on 10-04-20.
//

#include "LogReader.h"
#include <fstream>
#include "LogHeader.h"

unsigned long LogReader::fileMessageCount() {
    return index.size()-1;//We also index the very eof so it should be exactly one smaller!
}
void LogReader::close() {
    inStream->clear();
    inStream->close();
    delete inStream;
}
bool LogReader::open(const QString &fileName) {
    QByteArray fileNameBytes = fileName.toUtf8();
    const char * fname = fileNameBytes.data();
    inStream = new std::ifstream(fname, std::ios_base::in | std::ios_base::binary);
    if(!inStream->is_open()){
        std::cerr<<"Error opening log file \"" + fileName.toStdString() +"\"!"<<std::endl;
        return false;
    }
    LogFileHeader fileHeader;
    inStream->read((char*) &fileHeader, sizeof(fileHeader));
    if(strncmp(fileHeader.name,DEFAULT_LOGFILE_HEADER_NAME,sizeof(fileHeader.name))!= 0){
        std::cerr<<"Unrecognized logfile header"<<std::endl;
        return false;
    }else if(fileHeader.version!=0){
        std::cerr<<"Invalid log version!"<<std::endl;
        return false;
    }
    std::cout<<"Opened " + fileName.toStdString() + " as log version "+QString::number(fileHeader.version).toStdString()+ " (default is 0)"<<std::endl;
    return indexFile();
}

bool LogReader::indexFile() {
    long startPosition = inStream->tellg();
    LogDataHeader dataHeader;
    index.clear();
    int packetNumber = 0; //Upper limit is ~4.2 billion. This is more than sufficient.
    while(!inStream->eof() && inStream->is_open()){
        index[packetNumber] = inStream->tellg();
        inStream->read((char *)& dataHeader, sizeof(dataHeader));
        if(inStream->bad()){
            std::cerr<<"Error indexing file"<<std::endl;
            return false;
        }
        if(inStream->eof()){
            break; //note that we have also indexed the very end of the file, so our index is exactly one longer than
            //the amount of messages
        }
        inStream->seekg(long(inStream->tellg())+dataHeader.messageSize);
        packetNumber++;
    }
    inStream->clear();
    inStream->seekg(startPosition);
    return true;
}
std::pair<long long int, proto::FrameLog> LogReader::readPacket(long offset) {
    LogDataHeader dataHeader;
    inStream->seekg(offset);
    if(!inStream->eof()){
        inStream->read((char*) &dataHeader, sizeof(dataHeader));
        //very last data point is invalid at eof
        if(inStream->eof()){
            inStream->clear();
            return std::make_pair(-1,proto::FrameLog());
        }
        //Convert endian
        //read protobuf file data.
        char buffer[dataHeader.messageSize];
        inStream->read(buffer, dataHeader.messageSize);
        QByteArray data = QByteArray(buffer,dataHeader.messageSize);//set data
        proto::FrameLog logFrame;
        bool success = logFrame.ParseFromArray(data.data(),data.size());
        if (success){
            return std::make_pair(dataHeader.timestamp,logFrame);
        }else{
            return std::make_pair(-1,proto::FrameLog());
        }
    }
    inStream->clear();
    return std::make_pair(-1,proto::FrameLog());

}
std::pair<long long int, proto::FrameLog> LogReader::frameAt(int packet) {
    return readPacket(index[packet]);
}
std::pair<long long int, proto::FrameLog> LogReader::nextFrame() {
    return readPacket(inStream->tellg());
}