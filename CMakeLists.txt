cmake_minimum_required(VERSION 2.7)

project(functionCall)
add_definitions(${QT_DEFINITIONS} -fPIC -Wall -g -Werror  -std=c++0x )
include_directories(./src)
add_executable(functionCall ./example/example.cpp ./src/fcall.cpp)
target_link_libraries(functionCall  boost_regex)
