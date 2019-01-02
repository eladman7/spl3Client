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


        // pending notifications
        char* notifyBytes;
        bool success = connectionHandler.getBytes(notifyBytes, 2);
        if (success){
            short notifycode = connectionHandler.bytesToShort(notifyBytes)
            if (notifycode == 9){
                char* isPublicBytes;

                // pm/public
                connectionHandler.getBytes(isPublicBytes, 1);
                short isPublicShort = connectionHandler.bytesToShort(isPublicBytes);
                bool isPublic;
                if (isPublicShort == 0){
                    isPublic = false;
                }else if (isPublicShort == 1){
                    isPublic = true;
                } else{
                    std::cout << "invalid notification public/pm" << std::endl;
                    continue;
                }
                //
                char* postingUserBytes;
                connectionHandler.getLine()
                if (isPublic){

                }else{

                }

            }
        }

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
                // user name
                connectionHandler.sendFrameAscii(words[1], 0x0);
                // pass
                success_send = connectionHandler.sendFrameAscii(words[2], 0x0);
                if (success_send) {
                    std::cout << "woo" << std::endl;
                    char* opcodeResult;
                    connectionHandler.getBytes(opcodeResult, 2);
                    short opcode = connectionHandler.bytesToShort(opcodeResult);
                    if (opcode == 10){ // ack

                    } else if (opcode == 11){ // err

                    }

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


