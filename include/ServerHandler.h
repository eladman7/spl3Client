//
// Created by Elad Man on 2019-01-01.
//

#ifndef CLIENT_SERVERHANDLER_H
#define CLIENT_SERVERHANDLER_H

#include <mutex>
#include "SharedResourceInfo.h"

using namespace std;

class ServerHandler {
private:
    SharedResourceInfo &sharedResourceInfo;
public:
    ServerHandler(SharedResourceInfo &sharedResourceInfo);

    void run();

};


#endif //CLIENT_SERVERHANDLER_H