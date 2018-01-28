cmake_minimum_required(VERSION 3.6)
project(didactic_lang)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0")

set(SOURCE_FILES FSM/FSM.cpp FSM/FSM.h Prac1/Prac.h)
add_executable(didactic_lang ${SOURCE_FILES})
add_executable(prac Prac1/Prac.h Prac1/Prac.cpp)
add_executable(fsm_test FSM/FSM.cpp)
