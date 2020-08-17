cmake_minimum_required(VERSION 3.16)

project(comm-libs)

include(comm-libs.cmake)
include(ExternalProject)

option(BUILD_UT "Build unittest" ON)
option(BUILD_3PP "Build 3pp projects" ON)
option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)

add_custom_target(3pp)
if (BUILD_3PP STREQUAL ON)
    include(BuildGTest)
    include(BuildRapidjson)
endif ()
add_custom_target(3pp-clean COMMAND rm -rf ${3PP_BUILD_DIR}
                            COMMAND rm -rf ${3PP_INSTALL_DIR})

set(DEPEND_SRC ${CMAKE_BINARY_DIR}/libs)
include(libbase)
include(libutil)
include(liblog)
include(libfile)
include(libprocess)

add_custom_target(libs-clean COMMAND rm -rf ${DEPEND_SRC})

install(DIRECTORY ${DEPEND_SRC}/install/ DESTINATION ${CMAKE_INSTALL_PREFIX})
