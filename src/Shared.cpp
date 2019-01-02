#include <utility>

#include <utility>

//
// Created by Elad Man on 2019-01-01.
//


#include "../include/Shared.h"

using namespace std;

Shared::Shared(string userInput, mutex &mutex, ConnectionHandler &connectionHandler) :
    _mutex(mutex), connectionHandler(connectionHandler) {}


mutex &Shared::getMutex() {
    return _mutex;
}

void Shared::setShouldTerminate(bool terminate) {
    _shuoldTerminate = terminate;
}

bool Shared::shouldTerminate() {
    return _shuoldTerminate;
}


ConnectionHandler &Shared::getConnectionHandler() const {
    return connectionHandler;
}

void Shared::print(std::string x) {
    _mutex.lock();
    cout << x << endl;
    _mutex.unlock();
}


