cmake_minimum_required(VERSION 3.20)
project(MLFQ)

set(CMAKE_CXX_STANDARD 14)

add_executable(MLFQ lib/main.cpp lib/MLFQ.h)
