cmake_minimum_required(VERSION 3.16)
project(liblog)

include(../comm-libs.cmake)
include(ExternalProject)
set(PROJ_ROOT_PATH ${CMAKE_SOURCE_DIR})

option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)

INSTALL(FILES log_interface.h log_priority.h log_format.h log_device.h
        TYPE INCLUDE
        )

if(BUILD_TEST)
    add_subdirectory(test)
endif()
