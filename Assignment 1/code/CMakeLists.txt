cmake_minimum_required(VERSION 3.6)

#set(CMAKE_SYSTEM_NAME Linux)
#set(CMAKE_SYSTEM_PROCESSOR arm)
#set(CMAKE_C_COMPILER icc)
#set(CMAKE_CXX_COMPILER icpc)

project(assignment1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -restrict")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -restrict")

set(SOURCE_FILES
    dgemm.c
    gauss.c
    timer.c
    timer.h
    vector.c)

include_directories(.)

add_executable(assignment1 ${SOURCE_FILES})
