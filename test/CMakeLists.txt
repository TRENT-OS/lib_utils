#
# Unit tests
#
# Copyright (C) 2021, HENSOLDT Cyber GmbH
#

cmake_minimum_required(VERSION 3.17)

#-------------------------------------------------------------------------------
include("${TEST_MAIN_DIR}/test.cmake")
add_test_target(${PROJECT_NAME}
    SOURCES
        "src/Test_BitConverter.cpp"
        "src/Test_CharFifo.cpp"
        "src/Test_RleCompressor.cpp"
    MOCKS
        ext_mocks
        lib_mem_mocks
        lib_debug_mocks
)
