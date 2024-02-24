cmake_minimum_required(VERSION 3.14)
project(PokerClient)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES Client.cpp)
add_executable(PokerClient ${SOURCE_FILES})
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS network REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(PokerClient sfml-network)