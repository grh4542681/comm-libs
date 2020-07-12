cmake_minimum_required(VERSION 3.16)

project(comm-libs)

include(comm-libs.cmake)
include(ExternalProject)

option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)

set(EXTERNAL_PROJECT_CMAKE_ARGS)
list(APPEND EXTERNAL_PROJECT_CMAKE_ARGS "-DBUILD_TEST=${BUILD_TEST}")
list(APPEND EXTERNAL_PROJECT_CMAKE_ARGS "-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}")
if(OUT)
    list(APPEND EXTERNAL_PROJECT_CMAKE_ARGS "-DOUT=${OUT}")
endif()

ExternalProject_Add(libbase
                    SOURCE_DIR ${LIBBASE_SRC_TOP}
                    CONFIGURE_COMMAND ${CMAKE} ${EXTERNAL_PROJECT_CMAKE_ARGS} ${LIBBASE_SRC_TOP}
                    BUILD_COMMAND $(MAKE)
                    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                    )

ExternalProject_Add(libutil
                    SOURCE_DIR ${LIBUTIL_SRC_TOP}
                    CONFIGURE_COMMAND ${CMAKE} ${EXTERNAL_PROJECT_CMAKE_ARGS} ${LIBUTIL_SRC_TOP}
                    BUILD_COMMAND $(MAKE)
                    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                    )

ExternalProject_Add(libio
                    SOURCE_DIR ${LIBIO_SRC_TOP}
                    CONFIGURE_COMMAND ${CMAKE} ${EXTERNAL_PROJECT_CMAKE_ARGS} ${LIBIO_SRC_TOP}
                    BUILD_COMMAND $(MAKE)
                    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                    )

ExternalProject_Add(libfile
                    SOURCE_DIR ${LIBFILE_SRC_TOP}
                    CONFIGURE_COMMAND ${CMAKE} ${EXTERNAL_PROJECT_CMAKE_ARGS} ${LIBUTIL_SRC_TOP}
                    BUILD_COMMAND $(MAKE)
                    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                    )

ExternalProject_Add(libtimer
                    SOURCE_DIR ${LIBTIMER_SRC_TOP}
                    CONFIGURE_COMMAND ${CMAKE} ${EXTERNAL_PROJECT_CMAKE_ARGS} ${LIBTIMER_SRC_TOP}
                    BUILD_COMMAND $(MAKE)
                    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                    )
