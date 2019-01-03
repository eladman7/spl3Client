//
// Created by Elad Man on 2019-01-01.
//
#include <iostream>
#include <string>
#include <thread>
#include "../include/ServerHandler.h"
#include "../include/Shared.h"
#include <mutex>

using namespace std;


// main thread works with user keyboard input
int main(int argc, char *argv[]) {
    std::string host = argv[1];
    short port = atoi(argv[2]);
    string inputFromUser;
    std::mutex sharedMutex;
    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        cerr << "Cannot connect to " << host << ":" << port << endl;
        exit(1);
    }

    Shared shared = Shared(inputFromUser, sharedMutex, connectionHandler);

    ServerHandler serverHandler(shared);
    thread th1(&ServerHandler::listen, &serverHandler);

    while (!shared.shouldTerminate()) {
        string input;
//        shared.print("enter call for server:\n");
        std::getline(std::cin, input);
        serverHandler.send(input);
    }
}