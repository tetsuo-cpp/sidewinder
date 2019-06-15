cmake_minimum_required(VERSION 3.14)
project(sidewinder)

set(CMAKE_CXX_STANDARD 17)

set(SIDEWINDER_SOURCES Core.cpp Core.h Interfaces.h Server.cpp Server.h Connection.cpp Connection.h)

add_library(sidewinder ${SIDEWINDER_SOURCES})

add_executable(sidewinder_test ${SIDEWINDER_SOURCES} test_main.cpp)