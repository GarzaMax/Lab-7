cmake_minimum_required(VERSION 3.23)
project(labwork_7_GarzaMax)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(labwork_7_GarzaMax
        include/MP3Reader.cpp
        include/MP3Reader.h
        main.cpp)
