cmake_minimum_required(VERSION 3.16)
project(libbase)

include(../comm-libs.cmake)
include(ExternalProject)
set(PROJ_ROOT_PATH ${CMAKE_SOURCE_DIR})

option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)

INSTALL(FILES err_code.h return.h allocator.h
        TYPE INCLUDE
        )

if(BUILD_TEST)
    add_subdirectory(test)
endif()
