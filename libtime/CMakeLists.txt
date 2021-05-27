cmake_minimum_required(VERSION 3.16)
project(libtime VERSION 1.0.0
        DESCRIPTION "common time library")

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

    include(liblog)

    add_custom_target(depend-clean COMMAND rm -rf ${DEPEND_SRC})
endif ()

#project
set(LIBTIME_HRD ${PROJ_ROOT_PATH}
                 ${LIBBASE_SRC_TOP}
                 ${LIBLOG_SRC_TOP}
                 ${LIBUTIL_SRC_TOP}
                 ${LIBCONTAINER_SRC_TOP}
                 ${LIBSIGNAL_SRC_TOP}
                 ${LIBIO_SRC_TOP}
                 ${LIBTHREAD_SRC_TOP}
                 )
set(LIBTIME_SRC time_point.cc
                time_duration.cc
                )

set(LIBTIME_TARGETS)
if(BUILD_SHARED_LIBS)
    add_library(libtime.so SHARED ${LIBTIME_SRC})
    list(LENGTH DEPEND_PROJECTS DEPEND_PROJECTS_NUM)
    if (${DEPEND_PROJECTS_NUM})
        add_dependencies(libtime.so ${DEPEND_PROJECTS})
    endif()
    target_include_directories(libtime.so PRIVATE ${LIBTIME_HRD} ${DEPEND_INCLUDE_DIR})
    target_link_directories(libtime.so PRIVATE ${CMAKE_BINARY_DIR} ${DEPEND_LIB_DIR})
    target_link_libraries(libtime.so "")
    set_target_properties(libtime.so PROPERTIES OUTPUT_NAME "time" CLEAN_DIRECT_OUTPUT 1)
    list(APPEND LIBTIME_TARGETS libtime.so)
endif()

if(BUILD_STATIC_LIBS)
    add_library(libtime.a STATIC ${LIBTIME_SRC})
    list(LENGTH DEPEND_PROJECTS DEPEND_PROJECTS_NUM)
    if (${DEPEND_PROJECTS_NUM})
        add_dependencies(libtime.a ${DEPEND_PROJECTS})
    endif()
    target_include_directories(libtime.a PRIVATE ${LIBTIME_HRD} ${DEPEND_INCLUDE_DIR})
    target_link_directories(libtime.a PRIVATE ${CMAKE_BINARY_DIR} ${DEPEND_LIB_DIR})
    target_link_libraries(libtime.a "")
    set_target_properties(libtime.a PROPERTIES OUTPUT_NAME "time" CLEAN_DIRECT_OUTPUT 1)
    list(APPEND LIBTIME_TARGETS libtime.a)
endif()

add_custom_target(lib)
add_dependencies(lib ${LIBTIME_TARGETS})

INSTALL(FILES time_point.h
        TYPE INCLUDE
        )

INSTALL(TARGETS ${LIBTIME_TARGETS}
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
       )

include(CMakePackageConfigHelpers)
set(LIBTIME_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(LIBTIME_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")

configure_package_config_file(cmake/libtimeConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/libtimeConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
  PATH_VARS LIBTIME_INCLUDE_DIR LIBTIME_LIBRARY_DIR)
#write_basic_package_version_file(
#  ${CMAKE_CURRENT_BINARY_DIR}/FooConfigVersion.cmake
#  VERSION 1.2.3
#  COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libtimeConfig.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)

#test
if(BUILD_TEST AND BUILD_DEPEND_LIBS)
    add_subdirectory(test)
endif()

#ut
if(BUILD_UT)
    add_subdirectory(ut)
endif()
