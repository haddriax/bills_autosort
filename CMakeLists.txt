cmake_minimum_required(VERSION 3.22)
project(bills_autosort)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(bills_autosort main.cpp
        main.h
        AutoSorter.cpp
        AutoSorter.h)
