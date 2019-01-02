#include <stdlib.h>
#include <thread>
#include <boost/thread.hpp>
#include "../include/connectionHandler.h"
#include "../include/EncoderDecoder.h"
#include <mutex>

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
std::mutex mutex; // shared
std::string inputLine = ""; // shared
bool terminate; // shared

void start(std::string host, short port) {
    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        exit(1);
    }
    while (1) {
        if (inputLine == "") {
            mutex.lock(); // wait until input from user
        }
        std::string line = inputLine;
        inputLine = "";
        bool result = connectionHandler.sendFrameAscii(line, '\n');
        if (result) {
//            std::string answer = decode()
        }
        EncoderDecoder encoderDecoder = EncoderDecoder();
        // do everything here
        std::string response;
        if (response == "what we need to terminate i dont know") {
            terminate = true;
            fclose(stdin); // should break the wait of std::cin
            break;
        }
    }
}

//
//int main(int argc, char *argv[]) {
//    if (argc < 3) {
//        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
//        return -1;
//    }
//    std::string host = argv[1];
//    short port = atoi(argv[2]);
//    boost::thread communicationThread(start, host, port);
//    while (1) {
//        const short bufsize = 1024;
//        char buf[bufsize];
//        if (terminate) { // received terminate from communication thread
//            break;
//        }
//        if (inputLine == "") {  // we want another line
//            std::cin.getline(buf, bufsize);
//            std::string line(buf);
//            inputLine = line;
//            mutex.unlock(); // we have input
//        }
//        if (terminate) { // received terminate from communication thread
//            break;
//        }
//
//    }
//    return 0;
//}

