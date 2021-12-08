cmake_minimum_required(VERSION 3.20)
project(TPE_G1 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -pedantic -std=c99 -lm")

add_executable(TPE_G1 main.c imdbADT.c utils.c utils.h csvADT.c title.c title.h)
target_link_libraries(TPE_G1 m)