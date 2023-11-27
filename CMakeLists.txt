cmake_minimum_required(VERSION 3.26)
project(challenges_c C)

set(CMAKE_C_STANDARD 11)
configure_file(${CMAKE_SOURCE_DIR}/words.txt
        ${CMAKE_CURRENT_BINARY_DIR}/words.txt
        COPYONLY)
add_executable(challenges_c
        hangman.c)
