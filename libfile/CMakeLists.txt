cmake_minimum_required(VERSION 3.16)
project(libfile)

include(../comm-libs.cmake)
include(ExternalProject)
set(PROJ_ROOT_PATH ${CMAKE_SOURCE_DIR})

option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)
option(BUILD_DEPEND_LIBS "Build dependent Libraries" ON)

#dependent project
set(DEPEND_BINARY_DIR)
set(DEPEND_TARGETS)
set(DEPEND_PROJ_TARGETS)

#libutil
set(DEPEND_LIBUTIL_PROJ_NAME libutil)
set(DEPEND_LIBUTIL_TARGET libutil.so)
set(DEPEND_LIBUTIL_BINARY_DIR "depend/src/${DEPEND_LIBUTIL_PROJ_NAME}-build")
set(DEPEND_LIBUTIL_CMAKE_ARGS)
list(APPEND DEPEND_LIBUTIL_CMAKE_ARGS "-DBUILD_TEST=OFF")
list(APPEND DEPEND_LIBUTIL_CMAKE_ARGS "-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}")
list(APPEND DEPEND_LIBUTIL_CMAKE_ARGS "-DBUILD_DEPEND_LIBS=${BUILD_DEPEND_LIBS}")
if(OUT)
    list(APPEND DEPEND_LIBUTIL_CMAKE_ARGS "-DOUT=${OUT}")
endif()
ExternalProject_Add(${DEPEND_LIBUTIL_PROJ_NAME} PREFIX ${DEPEND_PREFIX}
                    SOURCE_DIR ${LIBUTIL_SRC_TOP}
                    BINARY_DIR ${DEPEND_LIBUTIL_BINARY_DIR}
                    CONFIGURE_COMMAND ${CMAKE} ${DEPEND_LIBUTIL_CMAKE_ARGS} ${LIBUTIL_SRC_TOP}
                    BUILD_COMMAND $(MAKE) ${DEPEND_LIBUTIL_TARGET}
                    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                    )
list(APPEND DEPEND_TARGETS ${DEPEND_LIBUTIL_TARGET})
list(APPEND DEPEND_PROJ_TARGETS ${DEPEND_LIBUTIL_PROJ_NAME})
list(APPEND DEPEND_BINARY_DIR "${CMAKE_BINARY_DIR}/${DEPEND_LIBUTIL_BINARY_DIR}")

#project
set(LIBFILE_HRD "${PROJ_ROOT_PATH}"
                "${LIBBASE_SRC_TOP}"
                "${LIBUTIL_SRC_TOP}"
                "${LIBIO_SRC_TOP}"
                )
set(LIBFILE_SRC file_api.cc
                file_fd.cc
                file.cc
                file_path.cc
                )

set(LIBFILE_TARGETS)

add_library(libfile.so SHARED ${LIBFILE_SRC})
target_include_directories(libfile.so PRIVATE ${LIBFILE_HRD})
set_target_properties(libfile.so PROPERTIES OUTPUT_NAME "file" CLEAN_DIRECT_OUTPUT 1)
list(APPEND LIBFILE_TARGETS libfile.so)

if(BUILD_STATIC_LIBS)
    add_library(libfile.a STATIC ${LIBFILE_SRC})
    target_include_directories(libfile.a PRIVATE ${LIBFILE_HRD})
    set_target_properties(libfile.a PROPERTIES OUTPUT_NAME "file" CLEAN_DIRECT_OUTPUT 1)
    list(APPEND LIBFILE_TARGETS libfile.a)
endif()

add_custom_target(lib)
add_dependencies(lib ${LIBFILE_TARGETS})
if(BUILD_DEPEND_LIBS)
    add_dependencies(lib ${DEPEND_PROJ_TARGETS})
endif()

INSTALL(FILES file.h
              file_log.h
              file_return.h
              file_fd.h
              file_api.h
              file_path.h
        TYPE INCLUDE
        )

INSTALL(TARGETS ${LIBFILE_TARGETS}
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
       )

if(BUILD_TEST)
    add_subdirectory(test)
endif()
