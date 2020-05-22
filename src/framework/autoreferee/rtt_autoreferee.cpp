//
// Created by rolf on 09-05-20.
//

#include "include/autoreferee/AutoReferee.h" //TODO: fix
#include <thread>
#include <QCoreApplication>
void runAutoReferee(){
  AutoReferee referee;
  referee.init();
  referee.run();
}
int main(int argc, char * argv[]){
  QCoreApplication app(argc, argv);
  std::thread applicationThread = std::thread(&runAutoReferee);
  applicationThread.join();
  return app.exec();
}