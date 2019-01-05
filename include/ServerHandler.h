//
// Created by Elad Man on 2019-01-01.
//

#ifndef CLIENT_SERVERHANDLER_H
#define CLIENT_SERVERHANDLER_H

#include <mutex>
#include "Shared.h"
#include "ConnectionHandler.h"

using namespace std;

class ServerHandler {
private:
    Shared &shared;
public:
    ServerHandler(Shared &sharedResourceInfo);

    void send(string input);
    void listen();

    short getNextShort(ConnectionHandler &connectionHandler) const;

    string getNextString(ConnectionHandler &connectionHandler) const;

    bool sendFollow(ConnectionHandler &handler, const vector<string> &words) const;

    void handleNotification(ConnectionHandler &handler, string &displayMe) const;

    void handleAck(ConnectionHandler &handler, string &displayMe) const;
};


#endif //CLIENT_SERVERHANDLER_H
