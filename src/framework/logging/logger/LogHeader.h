//
// Created by rolf on 10-04-20.
//

#ifndef SOCCER_LOGHEADER_H
#define SOCCER_LOGHEADER_H

#endif //SOCCER_LOGHEADER_H
static const char * DEFAULT_LOGFILE_HEADER_NAME = "RTT_LOG_FILE"; //In order to check if the file format is right.

struct LogFileHeader{
  char name[12]; // "SSL_LOG_FILE"
  int version; // Default format is version 0
};
struct LogDataHeader{
  long long int timestamp;
  int messageSize;
};