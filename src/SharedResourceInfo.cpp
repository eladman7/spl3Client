#include <utility>

#include <utility>

//
// Created by Elad Man on 2019-01-01.
//


#include "../include/SharedResourceInfo.h"

using namespace std;

SharedResourceInfo::SharedResourceInfo(string userInput, mutex &mutex, string host, short port) :
        _userInput(move(userInput)), _mutex(mutex), _host(std::move(host)), _port(port) {}

string &SharedResourceInfo::getUserInput() {
    return _userInput;
}

mutex &SharedResourceInfo::getMutex() {
    return _mutex;
}

string &SharedResourceInfo::getHost() {
    return _host;
}

short SharedResourceInfo::getPort() {
    return _port;
}

void SharedResourceInfo::setShouldTerminate(bool terminate) {
    _shuoldTerminate = terminate;
}

bool SharedResourceInfo::shouldTerminate() {
    return _shuoldTerminate;
}

void SharedResourceInfo::setUserInput(string &userInput) {
    _userInput = userInput;

}

