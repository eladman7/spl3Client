//
// Created by Elad Man on 30/12/2018.
//

#ifndef CLIENT_ENCODERDECODER_H
#define CLIENT_ENCODERDECODER_H
#include "../include/Controller.h"
#include "../include/connectionHandler.h"
#include <string>

class EncoderDecoder {

public:
    EncoderDecoder();
    char* encode(std::string stringToEncode);
    std::string decode(char bytes[]);
    static void start(Controller &controller, ConnectionHandler &connectionHandler);

private:
    short bytesToShort(char* bytesArr);
    void shortToBytes(short num, char* bytesArr);
};
#endif //CLIENT_ENCODERDECODER_H
