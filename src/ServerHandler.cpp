#include <utility>
#include <boost/algorithm/string.hpp>
//
// Created by Elad Man on 2019-01-01.
//

#include "../include/ServerHandler.h"
#include "../include/ConnectionHandler.h"
#include <mutex>

using namespace std;

void ServerHandler::send(string input) {
    ConnectionHandler &handler = shared.getConnectionHandler();

    string command;
    vector<string> words;
    boost::split(words, input, boost::is_any_of(" "));
    if (words.size() < 1) {
        return;
    }
    command = words[0];
    boost::to_upper(command);

    if (command == "REGISTER") {
        handler.sendShort(1); // user name
        handler.sendFrameAscii(words[1], 0x0);// pass
        handler.sendFrameAscii(words[2], 0x0);

    } else if (command == "LOGIN") {
        handler.sendShort(2); // user name
        handler.sendFrameAscii(words[1], 0x0); // pass
        handler.sendFrameAscii(words[2], 0x0);

    } else if (command == "LOGOUT") {
        handler.sendShort(3);

    } else if (command == "FOLLOW") {
        sendFollow(handler, words);

    } else if (command == "POST") {
        handler.sendShort(5);
        string content;
        content = input.substr(5, input.size());
        handler.sendFrameAscii(content, 0x0); // content

    } else if (command == "PM") {
        handler.sendShort(6);
        handler.sendFrameAscii(words[1], 0x0); // username
        string content;
        content = input.substr(3 + words[1].size() + 1, input.size());
        handler.sendFrameAscii(content, 0x0); // content
    } else if (command == "USERLIST") {
        handler.sendShort(7);

    } else if (command == "STAT") {
        handler.sendShort(8);
        handler.sendFrameAscii(words[1], 0x0); // username

    }

}

void ServerHandler::listen() {
    while (!shared.shouldTerminate()) {
        ConnectionHandler &handler = shared.getConnectionHandler();
        string displayMe;
        short opcode = getNextShort(handler);

        if (opcode == -1) {
            shared.setShouldTerminate(true);
            exit(0);

        } else if (opcode == 9) {
            handleNotification(handler, displayMe);

        } else if (opcode == 10) { // ack
            handleAck(handler, displayMe);

        } else if (opcode == 11) { // err
            short originOpcode = getNextShort(handler);
            displayMe += "ERROR " + to_string(originOpcode);
        }

        if (!displayMe.empty()) {
            cout << displayMe << endl;
            displayMe.clear();
        }
        if (shared.shouldTerminate()) {
            exit(0);
        }
    }
}

bool ServerHandler::sendFollow(ConnectionHandler &handler, const vector<string> &words) const {
    handler.sendShort(4);

    int isUnfollowInt = stoi(words[1]);
    if (isUnfollowInt == 0) {
        char buffer[1];
        buffer[0] = 0x0;
        handler.sendBytes(buffer, 1);

    } else if (isUnfollowInt == 1) {
        char buffer[1];
        buffer[0] = 0x1;
        handler.sendBytes(buffer, 1);
    } else {
        // illegal pm flag
        return false;
    }

    int numOfUsersInt = stoi(words[2]);
    short numOfUsers = (short) numOfUsersInt;

    bool success = false;
    success = handler.sendShort(numOfUsers);
    for (int j = 0; j < numOfUsers; ++j) {
        //usernames
        success = handler.sendFrameAscii(words[3 + j], 0x0);
    }
    return success;
}

string ServerHandler::getNextString(ConnectionHandler &connectionHandler) const {
    string postingUser;
    connectionHandler.getFrameAscii(postingUser, 0x0);
    return postingUser.substr(0, postingUser.size() - 1);  // cut the last 0 from string
}

short ServerHandler::getNextShort(ConnectionHandler &connectionHandler) const {
    char opcodeBytes[2];
    bool success = connectionHandler.getBytes(opcodeBytes, 2);
    if (success) {
        short opcode = connectionHandler.bytesToShort(opcodeBytes);
        return opcode;
    } else {
        return -1;
    }
}

ServerHandler::ServerHandler(Shared &sharedResourceInfo) : shared(sharedResourceInfo) {}

void ServerHandler::handleAck(ConnectionHandler &handler, string &displayMe) const {
    displayMe += "ACK";
    short originOpcode = getNextShort(handler);
    displayMe += " ";
    displayMe += to_string(originOpcode);
    // handle
    if (originOpcode == 4 or originOpcode == 7) { // follow or user lists
        short usersCount = getNextShort(handler);
        for (int j = 0; j < usersCount; ++j) {
            displayMe += " " + getNextString(handler);
        }

    } else if (originOpcode == 8) {
        short numPosts = getNextShort(handler);
        short numFollowers = getNextShort(handler);
        short numFollowing = getNextShort(handler);
        displayMe += " " + to_string(numPosts) + " " + to_string(numFollowers) + " " + to_string(numFollowing);

    } else if (originOpcode == 3) { // terminate
        shared.setShouldTerminate(true);
    }
}

void ServerHandler::handleNotification(ConnectionHandler &handler, string &displayMe) const {// pm/public
    char isPublicByte[1];
    handler.getBytes(isPublicByte, 1);

    if (isPublicByte[0] == 0x0) {
        displayMe += "PM";
    } else if (isPublicByte[0] == 0x1) {
        displayMe += "Public";
    } else {
        return;
    }

    //
    displayMe += " ";
    displayMe += getNextString(handler);

    displayMe += " ";
    displayMe += getNextString(handler);
}


