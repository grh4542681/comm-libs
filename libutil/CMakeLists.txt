cmake_minimum_required(VERSION 3.16)
project(libutil)

include(../comm-libs.cmake)

option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)

set(LIBUTIL_SRC string_util.cc
)

set(LIBUTIL_TARGETS)

add_library(GRHutil_SO SHARED ${LIBUTIL_SRC})
set_target_properties(GRHutil_SO PROPERTIES OUTPUT_NAME "util" CLEAN_DIRECT_OUTPUT 1)
set(LIBUTIL_TARGETS ${LIBUTIL_TARGETS} GRHutil_SO)

if(BUILD_STATIC_LIBS)
    add_library(GRHutil_A STATIC ${LIBUTIL_SRC})
    set_target_properties(GRHutil_A PROPERTIES OUTPUT_NAME "util" CLEAN_DIRECT_OUTPUT 1)
    set(LIBUTIL_TARGETS ${LIBUTIL_TARGETS} GRHutil_A)
endif()

INSTALL(TARGETS ${LIBUTIL_TARGETS}
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
       )
