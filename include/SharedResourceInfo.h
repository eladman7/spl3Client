//
// Created by Elad Man on 2019-01-01.
//

#ifndef CLIENT_SHAREDRESOURCEINFO_H
#define CLIENT_SHAREDRESOURCEINFO_H

#include <string>
#include <mutex>

using namespace std;

class SharedResourceInfo {
private:
    bool _shuoldTerminate=false;
    string _userInput;
    mutex &_mutex;
    short _port;
    string _host;
public:
    SharedResourceInfo(string userInput, mutex &mutex, string host, short port);

    string &getUserInput();
    void setUserInput(string &userInput);

    mutex &getMutex();

    string &getHost();

    short getPort();

    bool shouldTerminate();
    void setShouldTerminate(bool terminate);
};

#endif //CLIENT_SHAREDRESOURCEINFO_H
