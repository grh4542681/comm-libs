cmake_minimum_required(VERSION 3.16)
project(libio)

include(../comm-libs.cmake)
include(ExternalProject)
set(PROJ_ROOT_PATH ${CMAKE_SOURCE_DIR})

option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)

set(LIBIO_HRD ${PROJ_ROOT_PATH}
              ${LIBBASE_SRC_TOP}
              ${LIBLOG_SRC_TOP}
              )

set(LIBIO_SRC io_select.cc
              io_auto_select.cc
              io_select_item.cc
              io_epoll_fd.cc
              )

set(LIBIO_TARGETS)

##dependent project
#set(DEPEND_PREFIX depend)
#set(DEPEND_BINARY_DIR)
#set(DEPEND_TARGETS)
#
##liblog
#set(DEPEND_LIBLOG_NAME liblog)
#set(DEPEND_LIBLOG_BINARY_DIR "${DEPEND_PREFIX}/src/${DEPEND_LIBLOG_NAME}-build")
#set(DEPEND_LIBLOG_CMAKE_ARGS)
#list(APPEND DEPEND_LIBLOG_CMAKE_ARGS "-DBUILD_TEST=${BUILD_TEST}")
#list(APPEND DEPEND_LIBLOG_CMAKE_ARGS "-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}")
#if(OUT)
#    list(APPEND DEPEND_LIBLOG_CMAKE_ARGS "-DOUT=${OUT}")
#endif()
#ExternalProject_Add(${DEPEND_LIBLOG_NAME} PREFIX ${DEPEND_PREFIX}
#                    SOURCE_DIR ${LIBLOG_SRC_TOP}
#                    BINARY_DIR ${DEPEND_LIBLOG_BINARY_DIR}
#                    CONFIGURE_COMMAND ${CMAKE} ${DEPEND_LIBLOG_CMAKE_ARGS} ${LIBLOG_SRC_TOP}
#                    BUILD_COMMAND $(MAKE)
#                    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
#                    )
#list(APPEND DEPEND_TARGETS ${DEPEND_LIBLOG_NAME})


add_library(libio.so SHARED ${LIBIO_SRC})
set_target_properties(libio.so PROPERTIES OUTPUT_NAME "io" CLEAN_DIRECT_OUTPUT 1)
target_include_directories(libio.so PRIVATE ${LIBIO_HRD})
list(APPEND LIBIO_TARGETS libio.so)

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

if(BUILD_TEST)
    add_subdirectory(test)
endif()
