//
// Created by Elad Man on 2019-01-01.
//
#include <iostream>
#include <string>
#include <thread>
#include "../include/ServerHandler.h"
#include "../include/SharedResourceInfo.h"
#include <mutex>

using namespace std;


// main thread works with user keyboard input
int main(int argc, char *argv[]) {
    std::string host = argv[1];
    short port = atoi(argv[2]);
    string inputFromUser;
    std::mutex sharedMutex;
    SharedResourceInfo *sharedResourceInfo = new SharedResourceInfo(inputFromUser, sharedMutex, host, port);
    bool shouldTerminate = false;

    // start server handler
    ServerHandler serverHandler(*sharedResourceInfo);
    thread th1(&ServerHandler::run, &serverHandler);

    while (!sharedResourceInfo->shouldTerminate()) {
        string inp;
        std::cout << "enter call for server: ";
        std::getline(std::cin, inp);
        sharedResourceInfo->setUserInput(inp);
    }
}