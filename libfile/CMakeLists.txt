cmake_minimum_required(VERSION 3.16)
project(libfile)

include(../comm-libs.cmake)
include(ExternalProject)
set(PROJ_ROOT_PATH ${CMAKE_SOURCE_DIR})

option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)
option(BUILD_DEPEND_LIBS "Build dependent Libraries" ON)

option(AS_DEPEND_PROJ "As dependent" OFF)

#dependent project
message("--grh ${AS_DEPEND_PROJ}")
if (NOT AS_DEPEND_PROJ)
message("--grh ${AS_DEPEND_PROJ}")
    set(DEPEND_SRC ${CMAKE_BINARY_DIR}/depended)
endif ()

#libutil
include(depend_libutil)

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
