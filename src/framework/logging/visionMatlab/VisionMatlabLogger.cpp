//
// Created by rolf on 03-06-20.
//

#include "VisionMatlabLogger.h"
#include <iomanip>
#include <QDir>
#include <QDateTime>

bool VisionMatlabLogger::openFile(QString file) {
    QByteArray fileNameBytes = file.toUtf8();
    const char *fileName = fileNameBytes.constData();
    outStream = new std::ofstream(fileName,std::ios_base::out | std::ios_base::binary);
    if(!outStream->is_open()){
        std::cerr<<"Error opening MATLAB log file \""<< fileName<<"\"!"<<std::endl;
        return false;
    }else{
        std::cout<<"Writing to MATLAB log file \"" << fileName <<"\"!"<<std::endl;
    }
    (*outStream)<<std::setprecision(16);
    //set stream to decent precision to avoid problems when loading file in e.g. matlab.
    // We can raise this but there ain't much point for doubles

    return true;
}

void VisionMatlabLogger::closeFile() {
    outStream->clear();
    outStream->close();
    delete outStream;
}

void VisionMatlabLogger::writeData(int id, double time, const Eigen::VectorXd& observation,const Eigen::VectorXd& state,
                                   const Eigen::MatrixXd& covariance, const Eigen::VectorXd& innovation) {
    (*outStream) << id <<" "<<time<<" ";
    for (int i = 0; i < observation.size(); ++i) {
        (*outStream)<<observation[i]<<" ";
    }
    for (int i = 0; i < state.size(); ++i) {
        (*outStream)<<state[i]<<" ";
    }
    for (int j = 0; j < covariance.rows(); ++j) {
        for (int k = 0; k < covariance.cols(); ++k) {
            (*outStream)<<covariance(j,k)<<" ";
        }
    }
    for (int i = 0; i < innovation.size(); ++i) {
        (*outStream)<<innovation[i]<<" ";
    }
    (*outStream)<<std::endl;
}

int VisionMatlabLogger::writeNewFilter(int StateDim, int ObsDim, VisionMatlabLogger::FilterType type) {
    int marker = -1;
    int id = nextFilterID;
    assert(id!= -1);
    (*outStream) << marker<<" " << id << " " << StateDim << " " << ObsDim << " " << type <<std::endl;
    nextFilterID ++;
    return id;
}

void VisionMatlabLogger::startLogging() {
    if(isLogging){
        closeFile();
    }
    QDir directory = QDir::current();
    while(directory.dirName() != "soccer"){
        bool success = directory.cdUp();
        if(!success){
            std::cerr<<"Could not find application root folder, did you rename or are you not running from root?"<<std::endl;
            std::cerr<<"Could not open log file location"<<std::endl;
            return;
        }
    }
    if(!directory.cd("logfiles")){
        std::cout<<"Could not find logfiles directory, creating new directory"<<std::endl;
        directory.mkdir("logfiles");
        directory.cd("logfiles");
    }
    if(!directory.cd("matlab")){
        std::cout<<"Could not find matlab directory, creating new..."<<std::endl;
        directory.mkdir("matlab");
        directory.cd("matlab");
    }
    QString pathString = directory.absolutePath();
    QString timeString = QDateTime::currentDateTime().toString("/yyyy-MM-dd_hh-mm-ss-zzz");
    QString totalString = pathString+timeString+".mlog";
    std::cout<<totalString.toStdString()<<std::endl;
    isLogging = openFile(totalString);
    if(!isLogging){
        std::cerr<<"Failed to open new log file: "<<totalString.toStdString()<<std::endl;
    }
}

void VisionMatlabLogger::endLogging() {
    if(isLogging){
        closeFile();
        std::cout<<"Closed MATLAB log file"<<std::endl;
    }
    isLogging = false;
}

bool VisionMatlabLogger::isCurrentlyLogging() const{
    return isLogging;
}

namespace matlab_logger{
    VisionMatlabLogger logger;
}
