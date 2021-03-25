cmake_minimum_required(VERSION 3.16)
project(libio VERSION 1.0.0
        DESCRIPTION "common io library")

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

set(LIBIO_HRD ${PROJ_ROOT_PATH}
              ${LIBBASE_SRC_TOP}
              ${LIBLOG_SRC_TOP}
              ${LIBCONTAINER_SRC_TOP}
              ${LIBTIMER_SRC_TOP}
              ${LIBSIGNAL_SRC_TOP}
              ${LIBTHREAD_SRC_TOP}
              ${LIBPROCESS_SRC_TOP}
              )

set(LIBIO_SRC io_select.cc
              io_auto_select.cc
              io_select_item.cc
              io_epoll_fd.cc
              )

set(LIBIO_TARGETS)
if(BUILD_SHARED_LIBS)
    add_library(libio.so SHARED ${LIBIO_SRC})
    set_target_properties(libio.so PROPERTIES OUTPUT_NAME "io" CLEAN_DIRECT_OUTPUT 1)
    target_include_directories(libio.so PRIVATE ${LIBIO_HRD})
    list(APPEND LIBIO_TARGETS libio.so)
endif()

if(BUILD_STATIC_LIBS)
    add_library(libio.a STATIC ${LIBIO_SRC})
    set_target_properties(libio.a PROPERTIES OUTPUT_NAME "io" CLEAN_DIRECT_OUTPUT 1)
    target_include_directories(libio.a PRIVATE ${LIBIO_HRD})
    list(APPEND LIBIO_TARGETS libio.a)
endif()

add_custom_target(lib)
add_dependencies(lib ${LIBIO_TARGETS})

INSTALL(FILES io_log.h
              io_return.h
              io_defines.h
              io_fd.h
              io_epoll_fd.h
              io_select.h
              io_select_event.h
              io_select_item.h
              io_select_item_template.h
              io_auto_select.h
              io_fd_type.h
        TYPE INCLUDE
        )

INSTALL(TARGETS ${LIBIO_TARGETS}
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
       )

include(CMakePackageConfigHelpers)
set(LIBIO_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(LIBIO_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")

configure_package_config_file(cmake/libioConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/libioConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
  PATH_VARS LIBIO_INCLUDE_DIR LIBIO_LIBRARY_DIR)
#write_basic_package_version_file(
#  ${CMAKE_CURRENT_BINARY_DIR}/FooConfigVersion.cmake
#  VERSION 1.2.3
#  COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libioConfig.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)
