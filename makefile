COMPILE_FLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LINK_FLAGS:=-lboost_system -lpthread
# -fPIC -lrt -lboost_thread -c

all: BGSclient
	g++ -o bin/BGSclient bin/TwiterClient.o bin/ConnectionHandler.o bin/ServerHandler.o bin/Shared.o $(LINK_FLAGS)

BGSclient: bin/TwiterClient.o bin/ConnectionHandler.o bin/ServerHandler.o bin/Shared.o

bin/TwiterClient.o: src/TwiterClient.cpp
	g++ $(COMPILE_FLAGS) -o bin/TwiterClient.o src/TwiterClient.cpp

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(COMPILE_FLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/ServerHandler.o: src/ServerHandler.cpp
	g++ $(COMPILE_FLAGS) -o bin/ServerHandler.o src/ServerHandler.cpp

bin/Shared.o: src/Shared.cpp
	g++ $(COMPILE_FLAGS) -o bin/Shared.o src/Shared.cpp

.PHONY: clean
clean:
	rm -f bin/*
