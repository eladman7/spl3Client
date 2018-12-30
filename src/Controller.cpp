//
// Created by yonatan on 30/12/2018.
//

#include "../include/Controller.h"

bool Controller::isTerminate() const {
    return terminate;
}

void Controller::setTerminate(bool terminate) {
    this->terminate = terminate;
}
