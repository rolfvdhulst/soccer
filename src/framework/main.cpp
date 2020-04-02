//
// Created by rolf on 09-03-20.
//
#include <thread>
#include <QApplication>
#include <QStyleFactory>
#include "ApplicationManager.h"
#include "interface/include/interface/MainWindow.h" //TODO: fix include?
void setDarkTheme() {
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    qApp->setPalette(darkPalette);
    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}
void runApplication(bool &exitObject) {
    ApplicationManager manager;
    manager.init();
    manager.run(exitObject);
}
int main(int argc, char* argv[]) {
    bool exitObject = false;
    std::thread applicationThread = std::thread(&runApplication,std::ref(exitObject));
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc,argv);
    setDarkTheme();
    MainWindow * window = new MainWindow();
    window->show();
    int exit=app.exec();
    exitObject = true;
    applicationThread.join();
//    VisionLogReader reader;
//    bool succes=reader.open("/home/rolf/Downloads/testtesttest.log");
//    if(succes){
//        std::cout<<"SUCCESS"<<std::endl;
//    }else{
//        std::cout<<"FAILURE"<<std::endl;
//    }
//    std::cout<<reader.message().toStdString()<<std::endl;
//
//    for (int i = 0; i < reader.fileMessageCount(); ++ i) {
//        QByteArray data;
//        auto info = reader.nextVisionPacket(data);
//        if (info.second == MessageType::MESSAGE_SSL_VISION_2014){
//            proto::SSL_WrapperPacket packet;
//            bool success=packet.ParseFromArray(data,data.size());
//            if(!success){
//                std::cerr<<"could not parse packet: " << i<<std::endl;
//            }
//
//            //packet.PrintDebugString();
//        } else if( info.second == MessageType::MESSAGE_INVALID){
//            std::cerr<<"WTH: "<<i<<std::endl;
//        }
//    }
//    reader.close();
    return exit;
}