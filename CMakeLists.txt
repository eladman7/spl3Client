cmake_minimum_required(VERSION 3.10.2)
project(Client)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost 1.58.0 REQUIRED COMPONENTS
        filesystem thread)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(Client
            include/ConnectionHandler.h
            src/ConnectionHandler.cpp

            include/ServerHandler.h
            src/ServerHandler.cpp

            src/TwiterClient.cpp

            src/Shared.cpp
            include/Shared.h)
    target_link_libraries(Client ${Boost_LIBRARIES})
endif ()