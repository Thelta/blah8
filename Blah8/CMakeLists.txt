cmake_minimum_required(VERSION 3.1)

project(blah8 LANGUAGES C)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(blah8 source.c chip8.c chip8.h datastruct.c datastruct.h)

target_link_libraries(blah8 PRIVATE SDL2::SDL2 SDL2::SDL2main)