#include <utility>
#include <boost/algorithm/string.hpp>
//
// Created by Elad Man on 2019-01-01.
//

#include "../include/ServerHandler.h"
#include "../include/connectionHandler.h"
#include <mutex>

using namespace std;

void ServerHandler::send(string input) {
    ConnectionHandler &handler = shared.getConnectionHandler();

    string command;
    bool success_send;
    vector<string> words;
    boost::split(words, input , boost::is_any_of(" "));
    if (words.size() < 1){
        return;
    }
    command = words[0];
    boost::to_upper(command);

    if (command == "REGISTER"){
        handler.sendShort(1); // user name
        handler.sendFrameAscii(words[1], 0x0);// pass
        success_send = handler.sendFrameAscii(words[2], 0x0);

    } else if (command == "LOGIN"){
        handler.sendShort(2); // user name
        handler.sendFrameAscii(words[1], 0x0); // pass
        success_send = handler.sendFrameAscii(words[2], 0x0);

    } else if (command == "LOGOUT"){
        handler.sendShort(3);

    } else if (command == "FOLLOW"){
        sendFollow(handler, words);

    }else if (command == "POST"){
        handler.sendShort(5);
        handler.sendFrameAscii(words[1], 0x0); // content

    }else if (command == "PM"){
        handler.sendShort(6);
        handler.sendFrameAscii(words[1], 0x0); // username
        handler.sendFrameAscii(words[2], 0x0); // content

    }else if (command == "USERLIST"){
        handler.sendShort(7);

    }else if (command == "STAT"){
        handler.sendShort(8);
        handler.sendFrameAscii(words[1], 0x0); // username

    }

    if (success_send) {
        shared.print("success send");
    }

}

void ServerHandler::sendFollow(ConnectionHandler &handler, const vector<string> &words) const {
    handler.sendShort(4);

    int isUnfollowInt = stoi(words[1]);
    if (isUnfollowInt == 0){
            char buffer[1];
            buffer[0] = 0x0;
            handler.sendBytes(buffer, 1);

        } else if (isUnfollowInt == 1){
            char buffer[1];
            buffer[0] = 0x1;
            handler.sendBytes(buffer, 1);
        } else{
            shared.print("i hate you");
        }

    int numOfUsersInt = stoi(words[2]);
    short numOfUsers =(short) numOfUsersInt;

    handler.sendShort(numOfUsers);
    for (int j = 0; j < numOfUsers; ++j) {
            //usernames
            handler.sendFrameAscii(words[3+j], 0x0);
        }
}

string ServerHandler::getNextString(ConnectionHandler &connectionHandler) const {
    string postingUser;
    connectionHandler.getFrameAscii(postingUser, 0x0);
    return postingUser;
}

short ServerHandler::getNextShort(ConnectionHandler &connectionHandler) const {
    char* opcodeResult;
    bool success = connectionHandler.getBytes(opcodeResult, 2);
    if (success){
        short opcode = connectionHandler.bytesToShort(opcodeResult);
        return opcode;
    } else{
        return -1;
    }
}

ServerHandler::ServerHandler(Shared &sharedResourceInfo) : shared(sharedResourceInfo) {

}

void ServerHandler::listen() {
    while (!shared.shouldTerminate()){
        ConnectionHandler &handler = shared.getConnectionHandler();
        string displayMe;
        short opcode = getNextShort(handler);
        if (opcode == 9){
            // pm/public
            char* isPublicByte;
            handler.getBytes(isPublicByte, 1);
            short isPublicShort = handler.bytesToShort(isPublicByte);
            if (isPublicShort == 0){
                displayMe += "PM";
            }else if (isPublicShort == 1){
                displayMe += "Public";
            } else{
                shared.print("invalid notification public/pm");
                continue;
            }

            //
            string postingUser = getNextString(handler);
            displayMe += " ";
            displayMe += postingUser;

            string content = getNextString(handler);
            displayMe += " ";
            displayMe += content;

            cout << displayMe << endl;
            displayMe.clear();
            continue;
            //todo print this in the main thread
        }

        if (opcode == 10){ // ack
            displayMe += "ACK";
            short originOpcode = getNextShort(handler);
            displayMe += " ";
            displayMe += originOpcode;
            // handle
            if (originOpcode == 4 or originOpcode == 7){ // follow or user lists
                short usersCount = getNextShort(handler);
                for (int j = 0; j < usersCount; ++j) {

                    displayMe += " " + getNextString(handler);
                }
            } else if (originOpcode == 8){
                short numPosts = getNextShort(handler);
                short numFollowers = getNextShort(handler);
                short numFollowing = getNextShort(handler);
                displayMe += " " + to_string(numPosts)+ " " + to_string(numFollowers) + " " + to_string(numFollowing);

            } else if(originOpcode == 3){ // terminate
                shared.setShouldTerminate(true);
            }


        } else if (opcode == 11){ // err
            short originOpcode = getNextShort(handler);
            displayMe += "ERROR " + originOpcode;


        }
        cout << displayMe << endl;


    }
}


