CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
# LDFLAGS:=-lboost_system

all: Client
	g++ -o bin/TwiterClient.o bin/ConnectionHandler.o bin/ServerHandler.o bin/Shared.o -lboost_system -lpthread -lrt -c

Client: bin/TwiterClient.o bin/ConnectionHandler.o bin/ServerHandler.o bin/Shared.o

bin/TwiterClient.o: src/TwiterClient.cpp
	g++ $(CFLAGS) -o bin/TwiterClient.o src/TwiterClient.cpp

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/ServerHandler.o: src/ServerHandler.cpp
	g++ $(CFLAGS) -o bin/ServerHandler.o src/ServerHandler.cpp

bin/Shared.o: src/Shared.cpp
	g++ $(CFLAGS) -o bin/Shared.o src/Shared.cpp

.PHONY: clean
clean:
	rm -f bin/*
