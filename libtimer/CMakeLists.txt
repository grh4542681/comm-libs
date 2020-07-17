cmake_minimum_required(VERSION 3.16)
project(libtimer)

include(../comm-libs.cmake)
include(ExternalProject)
set(PROJ_ROOT_PATH ${CMAKE_SOURCE_DIR})

option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)

set(LIBTIMER_HRD ${PROJ_ROOT_PATH}
                 ${LIBBASE_SRC_TOP}
                 ${LIBLOG_SRC_TOP}
                 ${LIBIO_SRC_TOP}
                 )
set(LIBTIMER_SRC timer_fd.cc
                 )

set(LIBTIMER_TARGETS)

#dependent project
set(DEPEND_PREFIX depend)
set(DEPEND_BINARY_DIR)
set(DEPEND_TARGETS)

#liblog
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

add_library(libtimer.so SHARED ${LIBTIMER_SRC})
target_include_directories(libtimer.so PRIVATE ${LIBTIMER_HRD})
set_target_properties(libtimer.so PROPERTIES OUTPUT_NAME "gtimer" CLEAN_DIRECT_OUTPUT 1)
list(APPEND LIBTIMER_TARGETS libtimer.so)

if(BUILD_STATIC_LIBS)
    add_library(libtimer.a STATIC ${LIBTIMER_SRC})
    target_include_directories(libtimer.a PRIVATE ${LIBTIMER_HRD})
    set_target_properties(libtimer.a PROPERTIES OUTPUT_NAME "gtimer" CLEAN_DIRECT_OUTPUT 1)
    list(APPEND LIBTIMER_TARGETS libtimer.a)
endif()

add_custom_target(lib)
add_dependencies(lib ${LIBTIMER_TARGETS})

INSTALL(FILES timer_time.h timer_date.h timer_fd.h timer_return.h
        TYPE INCLUDE
        )

INSTALL(TARGETS ${LIBTIMER_TARGETS}
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
       )

if(BUILD_TEST)
    add_subdirectory(test)
endif()

