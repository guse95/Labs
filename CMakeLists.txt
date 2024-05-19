cmake_minimum_required(VERSION 3.26)
project(aborted)

set(CMAKE_CXX_STANDARD 23)

add_executable(aborted main.cpp
        limits.h
        database_struct.h
        database_functions.h
        kp_6.cpp
        kp_8.cpp
        kp_9.cpp)
