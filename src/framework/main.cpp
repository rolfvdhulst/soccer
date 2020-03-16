//
// Created by rolf on 09-03-20.
//
#include <pthread.h>
#include <thread>
#include "ApplicationManager.h"
void runApplication() {
    ApplicationManager manager;
    manager.init();
    manager.run();
}
int main(int argc, char* argv[]) {
    std::thread applicationThread = std::thread(&runApplication);
    while (true) {
    }
    return 1;
}