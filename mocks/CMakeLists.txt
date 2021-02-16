#
# Mocks provided for the unit test framework
#
# Copyright (C) 2021, HENSOLDT Cyber GmbH
#

cmake_minimum_required(VERSION 3.17)

#-------------------------------------------------------------------------------
add_library("${PROJECT_NAME}_mocks"
    "../src/PointerVector.c" # TODO Write a real mock.
)

target_include_directories("${PROJECT_NAME}_mocks"
    PUBLIC
        "../include"
)

target_link_libraries("${PROJECT_NAME}_mocks"
    PUBLIC
        lib_debug_mocks
        lib_mem_mocks
)
