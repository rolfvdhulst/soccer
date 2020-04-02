//
// Created by rolf on 01-04-20.
//

#ifndef SOCCER_VISIONLOGHEADER_H
#define SOCCER_VISIONLOGHEADER_H

#include "MessageType.h"
static const char * DEFAULT_FILE_HEADER_NAME = "SSL_LOG_FILE"; //this name is specified by the SSL so do not change it.

struct FileHeader{
  char name[12]; // "SSL_LOG_FILE"
  int version; // Default format is version 1
};
struct DataHeader{
  long long int timestamp;
  MessageType messageType;
  int messageSize;
};
#endif //SOCCER_VISIONLOGHEADER_H
