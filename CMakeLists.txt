cmake_minimum_required(VERSION 3.14)
project(sidewinder)

set(CMAKE_CXX_STANDARD 17)

set(SIDEWINDER_SOURCES
        Core.cpp Core.h Interfaces.h Server.cpp Server.h
        Connection.cpp Connection.h Client.cpp Client.h)

add_library(sidewinder ${SIDEWINDER_SOURCES})

add_executable(sidewinder_test_server ${SIDEWINDER_SOURCES} TestServer.cpp)
add_executable(sidewinder_test_client ${SIDEWINDER_SOURCES} TestClient.cpp)