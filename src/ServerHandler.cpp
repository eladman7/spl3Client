#include <utility>
#include <boost/algorithm/string.hpp>
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
            string input = sharedResourceInfo.getUserInput();
            string command;
            bool success_send;
            vector<string> words;
            boost::split(words, input , boost::is_any_of(" "));
            if (words.size() < 1){
                continue;
            } else {
                command = words[0];
                boost::to_upper(command);
            }
            if (command == "REGISTER"){
                connectionHandler.sendShort(1); // 1
                connectionHandler.sendFrameAscii(words[1], 0x0);
                success_send = connectionHandler.sendFrameAscii(words[2], 0x0);
                if (success_send) {
                    std::cout << "woo" << std::endl;
                }
            } else if (command == "LOGIN"){

            }


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


