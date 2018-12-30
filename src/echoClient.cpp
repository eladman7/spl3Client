#include <stdlib.h>
#include <thread>
#include <boost/thread.hpp>
#include "../include/connectionHandler.h"
#include "../include/EncoderDecoder.h"
#include "../include/Controller.h"

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
    
    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

    Controller controller = Controller();
    controller.setTerminate(false);

    EncoderDecoder encoderDecoder = EncoderDecoder();
    boost::thread communicationThread(encoderDecoder.start, controller, connectionHandler);

    while (1) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
		std::string line(buf);
		controller.setInput(line);
		if (controller.isTerminate()){ // received terminate from communication
            break;
		}

    }
    return 0;
}


