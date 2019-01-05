//
// Created by Elad Man on 2019-01-01.
//

#ifndef CLIENT_SHAREDRESOURCEINFO_H
#define CLIENT_SHAREDRESOURCEINFO_H

#include <string>
#include <mutex>
#include "ConnectionHandler.h"

using namespace std;

class Shared {
private:
    bool _shuoldTerminate=false;
    mutex &_mutex;
    ConnectionHandler &connectionHandler;

public:

    ConnectionHandler &getConnectionHandler() const;


public:
    Shared(string userInput, mutex &mutex, ConnectionHandler &connectionHandler);

    void print(std::string x);
    mutex &getMutex();

    bool shouldTerminate();
    void setShouldTerminate(bool terminate);
};

#endif //CLIENT_SHAREDRESOURCEINFO_H
