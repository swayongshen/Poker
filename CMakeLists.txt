cmake_minimum_required(VERSION 3.14)
project(RunPoker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fsanitize=address")

set(SOURCE_FILES main.cpp Deck.cpp Card.cpp Game.cpp
        Utils.h
        Utils.cpp
        Printer.cpp
        Printer.h
        ClientManager.cpp
        ClientManager.h
        Player.cpp
        Player.h)
add_executable(RunPoker ${SOURCE_FILES})
include_directories(/usr/include)

find_package(SFML 2.5 COMPONENTS network REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(RunPoker sfml-network)