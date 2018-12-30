//
// Created by yonatan on 30/12/2018.
//

#ifndef CLIENT_CONTROLLER_H
#define CLIENT_CONTROLLER_H

#include <string>

class Controller{
private:
    bool terminate;
    std::string input;
public:
    const std::string &getInput() const;

    void setInput(const std::string &input);

    bool isTerminate() const;

    void setTerminate(bool terminate);
};

#endif //CLIENT_CONTROLLER_H
