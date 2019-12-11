cmake_minimum_required(VERSION 3.0)
project(AdventOfCode2019 C)
enable_language(C)

set(CMAKE_C_FLAGS "-Wpedantic -Wall -Wextra -std=c99 -fsanitize=address -fno-omit-frame-pointer -g")

add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)
add_subdirectory(day4)
add_subdirectory(day5)
