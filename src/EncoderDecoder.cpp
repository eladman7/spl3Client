//
// Created by yonatan on 30/12/2018.
//

#include "../include/EncoderDecoder.h"
#include "../include/Controller.h"


void EncoderDecoder::start(Controller &controller) {
    while (1){
        // do everything here
        std::string response;
        if (response == "what we need to terminate i dont know"){
            controller.setTerminate(true);
            break;
        }
    }
}


