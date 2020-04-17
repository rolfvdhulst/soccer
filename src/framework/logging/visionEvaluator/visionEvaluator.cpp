//
// Created by rolf on 02-04-20.
//

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <visionLogger/VisionLogReader.h>
#include <protobuf/messages_robocup_ssl_wrapper.pb.h>
#include <visionFilter/VisionFilter.h>

int main(int argc, char* argv[]){
    QCoreApplication app(argc,argv);
    app.setApplicationName("Vision Evaluator");
    app.setOrganizationName("RoboTeam Twente");

    QCommandLineParser parser;
    parser.setApplicationDescription("Evaluates our vision system based on a vision log");
    parser.addHelpOption();
    parser.addPositionalArgument("source file","vision log file to read");
    parser.process(app);

    int argCount = parser.positionalArguments().size();
    if (argCount <1){
        parser.showHelp(1);
        return 1;
    }
    VisionLogReader reader;
    bool success = reader.open(parser.positionalArguments().first());
    if(!success){
        return 1;
    }

    QByteArray visionData;
    MessageType messageType = MessageType::MESSAGE_BLANK;
    std::vector<proto::SSL_WrapperPacket> packets;
    while(messageType != MessageType::MESSAGE_INVALID){
        auto packetData = reader.nextVisionPacket(visionData); // at eof we get MessageType::Message_INVALID
        long long receiveTime = packetData.first; //in nano seconds
        messageType = packetData.second;
        if(messageType == MessageType::MESSAGE_SSL_VISION_2014){
            proto::SSL_WrapperPacket packet;
            bool parseSuccess=packet.ParseFromArray(visionData.data(),visionData.size());
            if(!parseSuccess){
                std::cerr<<"Failed parsing packet!!"<<std::endl;
                return 1;
            }
            packets.emplace_back(std::move(packet));
        }
    }

    VisionFilter filter;

    double estimationError = 0;
    double estimationErrorSq = 0;
    double afterMeasurementError = 0;
    double afterMeasurementErrorSq = 0;
    int beforeCount = 0;
    int afterCount = 0;
    Time startAnalysis = Time::now();
    unsigned long percentage = 0;
    unsigned long totalSize = packets.size();
    for (unsigned long i = 0; i < packets.size(); i++){
        const auto& packet = packets[i];
        if(packet.has_detection()){
            auto prePacketWorld = filter.process({},Time(packet.detection().t_capture()));
            if (packet.detection().balls_size() == 1 && prePacketWorld.has_ball()){
                Vector2 detectionPos(packet.detection().balls(0).x()*0.001,packet.detection().balls(0).y()*0.001);
                Vector2 worldPos(prePacketWorld.ball().pos());
                estimationError+=(detectionPos-worldPos).length();
                estimationErrorSq+=(detectionPos-worldPos).length2();
                beforeCount++;
            }
            auto postPacketWorld = filter.process({packet},Time(packet.detection().t_capture()));
            if (packet.detection().balls_size() == 1 && postPacketWorld.has_ball()){
                Vector2 detectionPos(packet.detection().balls(0).x()*0.001,packet.detection().balls(0).y()*0.001);
                Vector2 worldPos(postPacketWorld.ball().pos());
                afterMeasurementError+=(detectionPos-worldPos).length();
                afterMeasurementErrorSq+=(detectionPos-worldPos).length2();
                afterCount++;
            }
        }
        unsigned long newPercentage = i*100/totalSize;
        if(newPercentage>percentage){
            percentage = newPercentage;

            std::cout<<"[";
            for (int j = 0; j < 100; ++ j) {
                std::cout<< (j<percentage ? "=" : " ");
            }
            std::cout<<"] " << percentage<<" % ";
            std::cout<<"after "<< startAnalysis.timeSince().asSeconds()<<" s\r"<<std::flush;
        }
    }
    std::cout<<std::endl;

    std::cout<<"preMeasurement error: " <<std::endl;
    std::cout<<"error: "<<estimationError<<" squared error: " <<estimationErrorSq<<" with "<<beforeCount<<" measurements"<<std::endl<<std::endl;
    std::cout<<"postMeasurement error: " <<std::endl;
    std::cout<<"error: "<<afterMeasurementError<<" squared error: " <<afterMeasurementErrorSq<<" with "<<afterCount<<" measurements"<<std::endl;

    return 0;
}