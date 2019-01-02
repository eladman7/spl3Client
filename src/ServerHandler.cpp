#include <utility>
#include <boost/algorithm/string.hpp>
//
// Created by Elad Man on 2019-01-01.
//

#include "../include/ServerHandler.h"
#include "../include/connectionHandler.h"
#include <mutex>

using namespace std;

void ServerHandler::run() {
    ConnectionHandler connectionHandler(sharedResourceInfo.getHost(), sharedResourceInfo.getPort());
    if (!connectionHandler.connect()) {
        cerr << "Cannot connect to " << sharedResourceInfo.getHost() << ":" << sharedResourceInfo.getPort()
                  << endl;
        exit(1);
    }
    while (!sharedResourceInfo.shouldTerminate()) {
        lock_guard<mutex> lock(sharedResourceInfo.getMutex());
        string displayMe;

        // pending notifications
        char* notifyBytes;
        bool success = connectionHandler.getBytes(notifyBytes, 2);
        if (success){
            short notifycode = connectionHandler.bytesToShort(notifyBytes);
            if (notifycode == 9){
                char* isPublicBytes;

                // pm/public
                connectionHandler.getBytes(isPublicBytes, 1);
                short isPublicShort = connectionHandler.bytesToShort(isPublicBytes);
                if (isPublicShort == 0){
                    displayMe += "PM";
                }else if (isPublicShort == 1){
                    displayMe += "Public";
                } else{
                    cout << "invalid notification public/pm" << endl;
                    continue;
                }

                //
                string postingUser = getNextString(connectionHandler);
                displayMe += " ";
                displayMe += postingUser;

                string content = getNextString(connectionHandler);
                displayMe += " ";
                displayMe += content;
                cout << displayMe << endl;
                displayMe.clear();
                continue;
                //todo print this in the main thread
            }
        }

        if (!sharedResourceInfo.getUserInput().empty()) {
            // call server
            string input = sharedResourceInfo.getUserInput();
            string command;
            bool success_send;
            vector<string> words;
            boost::split(words, input , boost::is_any_of(" "));
            command = words[0]; // will not fail
            boost::to_upper(command);

            if (command == "REGISTER"){
                connectionHandler.sendShort(1); // user name
                connectionHandler.sendFrameAscii(words[1], 0x0);// pass
                success_send = connectionHandler.sendFrameAscii(words[2], 0x0);

            } else if (command == "LOGIN"){
                connectionHandler.sendShort(2); // user name
                connectionHandler.sendFrameAscii(words[1], 0x0); // pass
                success_send = connectionHandler.sendFrameAscii(words[2], 0x0);

            } else if (command == "LOGOUT"){
                connectionHandler.sendShort(3);

            } else if (command == "FOLLOW"){
                connectionHandler.sendShort(4);

                int isUnfollowInt = stoi(words[1]);
                if (isUnfollowInt == 0){
                    char buffer[1];
                    buffer[0] = 0x0;
                    connectionHandler.sendBytes(buffer, 1);

                } else if (isUnfollowInt == 1){
                    char buffer[1];
                    buffer[0] = 0x1;
                    connectionHandler.sendBytes(buffer, 1);
                } else{
                    cout << "i hate you" << endl;
                    sharedResourceInfo.getUserInput().clear();
                    continue;
                }

                int numOfUsersInt = std::stoi(words[2]);
                short numOfUsers =(short) numOfUsersInt;

                connectionHandler.sendShort(numOfUsers);
                for (int j = 0; j < numOfUsers; ++j) {
                    //usernames
                    connectionHandler.sendFrameAscii(words[3+j], 0x0);
                }


            }else if (command == "POST"){
                connectionHandler.sendShort(5);
                connectionHandler.sendFrameAscii(words[1], 0x0); // content

            }else if (command == "PM"){
                connectionHandler.sendShort(6);
                connectionHandler.sendFrameAscii(words[1], 0x0); // username
                connectionHandler.sendFrameAscii(words[2], 0x0); // content

            }else if (command == "USERLIST"){
                connectionHandler.sendShort(7);

            }else if (command == "STAT"){
                connectionHandler.sendShort(8);
                connectionHandler.sendFrameAscii(words[1], 0x0); // username

            }else { // wrong command
                sharedResourceInfo.getUserInput().clear();
                continue;
            }


            // handle response
            if (success_send) {
                cout << "woo" << endl;
                short opcode = getNextShort(connectionHandler);
                if (opcode == 10){ // ack
                    displayMe += "ACK";
                    short originOpcode = getNextShort(connectionHandler);
                    displayMe += " ";
                    displayMe += originOpcode;
                    // handle
                    if (originOpcode == 4 or originOpcode == 7){ // follow or user lists
                        short usersCount = getNextShort(connectionHandler);
                        for (int j = 0; j < usersCount; ++j) {

                            displayMe += " " + getNextString(connectionHandler);
                        }
                    } else if (originOpcode == 8){
                        short numPosts = getNextShort(connectionHandler);
                        short numFollowers = getNextShort(connectionHandler);
                        short numFollowing = getNextShort(connectionHandler);
                        displayMe += " " + to_string(numPosts)+ " " + to_string(numFollowers) + " " + to_string(numFollowing);

                    } else if(originOpcode == 3){ // terminate
                        sharedResourceInfo.setShouldTerminate(true);
                    }



                } else if (opcode == 11){ // err
                    short originOpcode = getNextShort(connectionHandler);
                    displayMe += "ERROR " + originOpcode;


                }
                cout << displayMe << endl;

            }


            if (sharedResourceInfo.shouldTerminate()){
                break;
            }

            sharedResourceInfo.getUserInput().clear();
        }
        sharedResourceInfo.getMutex().unlock();
    }
}

string ServerHandler::getNextString(ConnectionHandler &connectionHandler) const {
    string postingUser;
    connectionHandler.getFrameAscii(postingUser, 0x0);
    return postingUser;
}

short ServerHandler::getNextShort(ConnectionHandler &connectionHandler) const {
    char* opcodeResult;
    connectionHandler.getBytes(opcodeResult, 2);
    short opcode = connectionHandler.bytesToShort(opcodeResult);
    return opcode;
}

ServerHandler::ServerHandler(SharedResourceInfo &sharedResourceInfo) : sharedResourceInfo(sharedResourceInfo) {

}


