//
// Created by yonatan on 30/12/2018.
//

#include <iostream>
#include <boost/integer_fwd.hpp>
#include "../include/EncoderDecoder.h"
#include "../include/connectionHandler.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>



std::string EncoderDecoder::decode(char *bytes) {
    short returnOptCode = bytesToShort(bytes);
    std::string result;
    if (returnOptCode == 9) {
        result += "Notification";
    } else if (returnOptCode == 10) {
        result += "ACK";
    } else if (returnOptCode == 11) {
        result += "ERROR " + std::to_string(returnOptCode);
    }
    return result;
}

char *EncoderDecoder::encode(std::string stringToEncode) {
    return nullptr;
}

short EncoderDecoder::bytesToShort(char *bytesArr) {
    short result = (short) ((bytesArr[0] & 0xff) << 8);
    result += (short) (bytesArr[1] & 0xff);
    return result;
}

void EncoderDecoder::shortToBytes(short num, char *bytesArr) {
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}
