#include <utility>

//
// Created by Elad Man on 2019-01-01.
//

#include "../include/ServerHandler.h"
#include "../include/connectionHandler.h"
#include <mutex>

using namespace std;

void ServerHandler::run() {
    ConnectionHandler connectionHandler(sharedResourceInfo.getHost(), sharedResourceInfo.getPort());
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << sharedResourceInfo.getHost() << ":" << sharedResourceInfo.getPort()
                  << std::endl;
        exit(1);
    }
    while (!sharedResourceInfo.shouldTerminate()) {
        lock_guard<mutex> lock(sharedResourceInfo.getMutex());
        if (!sharedResourceInfo.getUserInput().empty()) {
            // call server
            bool res = connectionHandler.sendLine(sharedResourceInfo.getUserInput());
            std::cerr << res << std::endl;
            // todo: handle response
            // 1. print it correctly
            // 2. stop raise shuold terminate if got ack from Logout command
            // todo - replace this breaking condition with ack for logout
            if (sharedResourceInfo.getUserInput() == "stop") {
//                sharedResourceInfo.setShouldTerminate(true);
                break;
            }
            sharedResourceInfo.getUserInput().clear();
        }
        sharedResourceInfo.getMutex().unlock();
    }
}

ServerHandler::ServerHandler(SharedResourceInfo &sharedResourceInfo) : sharedResourceInfo(sharedResourceInfo) {

}


