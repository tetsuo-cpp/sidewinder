cmake_minimum_required(VERSION 3.13)
project(sidewinder)

set(CMAKE_CXX_STANDARD 17)

set(SIDEWINDER_SOURCES
        Core.cpp Core.h Interfaces.h Server.cpp Server.h
        Connection.cpp Connection.h Client.cpp Client.h
        Timer.cpp Timer.h Address.cpp Address.h)

add_library(sidewinder ${SIDEWINDER_SOURCES})

add_executable(sidewinder_test_server TestServer.cpp)
target_link_libraries(sidewinder_test_server sidewinder)

add_executable(sidewinder_test_client TestClient.cpp)
target_link_libraries(sidewinder_test_client sidewinder)
