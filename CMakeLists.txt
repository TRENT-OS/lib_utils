#
# Utilities Library
#
# Copyright (C) 2020, HENSOLDT Cyber GmbH
#

cmake_minimum_required(VERSION 3.17)

#-------------------------------------------------------------------------------
project(lib_utils C)

#-------------------------------------------------------------------------------
# LIBRARY
#-------------------------------------------------------------------------------
add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME}
    INTERFACE
        "src/BitConverter.c"
        "src/CharFifo.c"
        "src/PointerVector.c"
        "src/RleCompressor.c"
)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        "include"
)

if (DEBUG_CONFIG_H_FILE)
    target_compile_definitions(${PROJECT_NAME}
        INTERFACE
            "DEBUG_CONFIG_H_FILE=${DEBUG_CONFIG_H_FILE}"
    )
endif ()

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        os_core_api
        lib_compiler
        lib_debug
        lib_mem
)

#-------------------------------------------------------------------------------
# TESTING
#-------------------------------------------------------------------------------
if (BUILD_TESTING)
    add_subdirectory(test)
    add_subdirectory(mocks)
endif ()
