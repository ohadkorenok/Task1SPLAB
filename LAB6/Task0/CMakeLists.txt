cmake_minimum_required(VERSION 3.12)
project(Task0 C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)            # Add main.cpp file of project root directory as source file
add_executable(taskFile ${SOURCE_FILES})       # Add executable target with source files listed in SOURCE_FILES variable

add_library(Task1 main.c)