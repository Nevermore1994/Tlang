
cmake_minimum_required(VERSION 2.8)

#project
project(Tlang)

add_subdirectory(core)

aux_source_directory(. DIR_SRCS)

add_executable(Tlang main.cpp)