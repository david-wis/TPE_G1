cmake_minimum_required(VERSION 3.20)
project(TPE_G1 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -pedantic -std=c99 -fsanitize=address -lm -g")

add_executable(TPE_G1 main.c imdbADT.c utils.c csvADT.c title.c)
target_link_libraries(TPE_G1 m)