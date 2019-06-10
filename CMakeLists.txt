cmake_minimum_required(VERSION 3.14)
project(sidewinder)

set(CMAKE_CXX_STANDARD 17)

set(SIDEWINDER_SOURCES Core.cpp Core.h Interfaces.h Server.cpp Server.h)

add_library(sidewinder ${SIDEWINDER_SOURCES})