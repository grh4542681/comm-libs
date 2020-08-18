cmake_minimum_required(VERSION 3.16)
project(libutil VERSION 1.0.0
        DESCRIPTION "common utils library")

if(DEFINED ENV{REPOTOP})
    include($ENV{REPOTOP}/comm-libs.cmake)
else ()
    message(FATAL_ERROR "Env REPOTOP not set")
endif ()

set(PROJ_ROOT_PATH ${CMAKE_SOURCE_DIR})

option(BUILD_UT "Build unit test" ON)
option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)
option(BUILD_SHARED_LIBS "Build shared Libraries" ON)
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

#project
set(LIBUTIL_HRD "${PROJ_ROOT_PATH}"
                "${LIBBASE_SRC_TOP}"
                )

set(LIBUTIL_SRC string_util.cc
)

set(LIBUTIL_TARGETS)
if(BUILD_SHARED_LIBS)
    add_library(libutil.so SHARED ${LIBUTIL_SRC})
    target_include_directories(libutil.so PRIVATE ${LIBUTIL_HRD})
    set_target_properties(libutil.so PROPERTIES OUTPUT_NAME "util" CLEAN_DIRECT_OUTPUT 1)
    set(LIBUTIL_TARGETS ${LIBUTIL_TARGETS} libutil.so)
endif()

if(BUILD_STATIC_LIBS)
    add_library(libutil.a STATIC ${LIBUTIL_SRC})
    target_include_directories(libutil.a PRIVATE ${LIBUTIL_HRD})
    set_target_properties(libutil.a PROPERTIES OUTPUT_NAME "util" CLEAN_DIRECT_OUTPUT 1)
    set(LIBUTIL_TARGETS ${LIBUTIL_TARGETS} libutil.a)
endif()
add_custom_target(lib)
add_dependencies(lib ${LIBUTIL_TARGETS})
if(BUILD_DEPEND_LIBS AND DEPEND_PROJ_TARGETS)
    add_dependencies(lib ${DEPEND_PROJ_TARGETS})
endif()

#install

INSTALL(FILES string_util.h
        TYPE INCLUDE
        )

INSTALL(TARGETS ${LIBUTIL_TARGETS}
       )

include(CMakePackageConfigHelpers)
set(LIBUTIL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(LIBUTIL_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")

configure_package_config_file(cmake/libutilConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/libutilConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
  PATH_VARS LIBUTIL_INCLUDE_DIR LIBUTIL_LIBRARY_DIR)
#write_basic_package_version_file(
#  ${CMAKE_CURRENT_BINARY_DIR}/FooConfigVersion.cmake
#  VERSION 1.2.3
#  COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libutilConfig.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)

#ut
if(BUILD_UT)
    add_subdirectory(ut)
endif()
