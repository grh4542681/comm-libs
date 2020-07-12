cmake_minimum_required(VERSION 3.16)
project(libio)

include(../comm-libs.cmake)
include(ExternalProject)
set(PROJ_ROOT_PATH ${CMAKE_SOURCE_DIR})

option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)

#set(LIBIO_HRD "${PLAT_INFRA_PATH}/return"
#               "${PLAT_INFRA_PATH}/base"
#               "${PLAT_INFRA_PATH}/timer"
#               "${PLAT_INFRA_PATH}/process"
#               "${PLAT_INFRA_PATH}/thread"
#               "${PLAT_INFRA_PATH}/io"
#               )
#set(LIBIO_SRC io_return.cc
#               io_fd.cc
#               io_select.cc
#               io_auto_select.cc
#               io_select_item.cc
#               io_epoll_fd.cc
#               )
#
#add_library(GRHio_SO SHARED ${LIBIO_SRC})
#set_target_properties(GRHio_SO PROPERTIES OUTPUT_NAME "gio" CLEAN_DIRECT_OUTPUT 1)
#target_include_directories(GRHio_SO PRIVATE ${LIBIO_HRD})
#
#if(BUILD_STATIC_LIBS)
#    add_library(GRHio_A STATIC ${LIBIO_SRC})
#    set_target_properties(GRHio_A PROPERTIES OUTPUT_NAME "gio" CLEAN_DIRECT_OUTPUT 1)
#    target_include_directories(GRHio_A PRIVATE ${LIBIO_HRD})
#endif()
if(BUILD_TEST)
    add_subdirectory(test)
endif()
