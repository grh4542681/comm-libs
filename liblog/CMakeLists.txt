cmake_minimum_required(VERSION 3.16)
project(liblog VERSION 1.0.0
        DESCRIPTION "common log library")

if(DEFINED ENV{REPOTOP})
    include($ENV{REPOTOP}/comm-libs.cmake)
else ()
    message(FATAL_ERROR "Env REPOTOP not set")
endif ()

set(PROJ_ROOT_PATH ${CMAKE_SOURCE_DIR})

option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)
option(BUILD_DEPEND_LIBS "Build dependent Libraries" ON)

option(INTERNAL_AS_DEPEND_PROJ "As dependent" OFF)

#dependent project
if (BUILD_DEPEND_LIBS STREQUAL ON)
    if (INTERNAL_AS_DEPEND_PROJ STREQUAL OFF)
        set(DEPEND_SRC ${CMAKE_BINARY_DIR}/depended)
    endif ()
    set(DEPEND_PROJECTS)

    add_custom_target(depend-clean COMMAND rm -rf ${DEPEND_SRC})
endif ()

INSTALL(FILES log_interface.h log_priority.h log_format.h log_device.h
        TYPE INCLUDE
        )

include(CMakePackageConfigHelpers)
set(LIBLOG_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(LIBLOG_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")

configure_package_config_file(cmake/liblogConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/liblogConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
  PATH_VARS LIBLOG_INCLUDE_DIR LIBLOG_LIBRARY_DIR)
#write_basic_package_version_file(
#  ${CMAKE_CURRENT_BINARY_DIR}/FooConfigVersion.cmake
#  VERSION 1.2.3
#  COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/liblogConfig.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)
