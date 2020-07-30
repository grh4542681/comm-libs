cmake_minimum_required(VERSION 3.16)
project(libutil)

include(../comm-libs.cmake)

option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)
option(BUILD_DEPEND_LIBS "Build dependent Libraries" ON)

#dependent project
set(DEPEND_TARGETS)
set(DEPEND_BINARY_DIR)
set(DEPEND_PROJ_TARGETS)

#project
set(LIBUTIL_SRC string_util.cc
)

set(LIBUTIL_TARGETS)

add_library(libutil.so SHARED ${LIBUTIL_SRC})
set_target_properties(libutil.so PROPERTIES OUTPUT_NAME "util" CLEAN_DIRECT_OUTPUT 1)
set(LIBUTIL_TARGETS ${LIBUTIL_TARGETS} libutil.so)

if(BUILD_STATIC_LIBS)
    add_library(GRHutil_A STATIC ${LIBUTIL_SRC})
    set_target_properties(GRHutil_A PROPERTIES OUTPUT_NAME "util" CLEAN_DIRECT_OUTPUT 1)
    set(LIBUTIL_TARGETS ${LIBUTIL_TARGETS} GRHutil_A)
endif()

add_custom_target(lib)
add_dependencies(lib ${LIBUTIL_TARGETS})
if(BUILD_DEPEND_LIBS AND DEPEND_PROJ_TARGETS)
    add_dependencies(lib ${DEPEND_PROJ_TARGETS})
endif()

INSTALL(FILES string_util.h
        TYPE INCLUDE
        )

INSTALL(TARGETS ${LIBUTIL_TARGETS}
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
       )
