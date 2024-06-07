cmake_minimum_required(VERSION 3.29)
project(chip_8)

set(CMAKE_CXX_STANDARD 11)

add_executable(chip_8
        src/mem.h
        src/cpu.h
        src/main.cpp
        src/cpu.cpp
        src/mem.cpp
        src/util.h
        src/util.cpp
        src/display.h
        src/display.cpp
)
