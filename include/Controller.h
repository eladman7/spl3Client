//
// Created by yonatan on 30/12/2018.
//

#ifndef CLIENT_CONTROLLER_H
#define CLIENT_CONTROLLER_H

class Controller{
private:
    bool terminate;
public:
    bool isTerminate() const;

    void setTerminate(bool terminate);
};

#endif //CLIENT_CONTROLLER_H
