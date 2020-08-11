cmake_minimum_required(VERSION 3.16)
project(libprocess)

include(../comm-libs.cmake)
include(ExternalProject)
set(PROJ_ROOT_PATH ${CMAKE_SOURCE_DIR})

option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)
option(BUILD_DEPEND_LIBS "Build dependent Libraries" ON)
option(BUILD_INTER_DEPEND_LIBS "Build dependnt Libraries's dependent" ON)

#project
set(LIBPROCESS_HRD ${PROJ_ROOT_PATH}
                   ${LIBBASE_SRC_TOP}
                   ${LIBLOG_SRC_TOP}
                   ${LIBIO_SRC_TOP}
                   ${LIBFILE_SRC_TOP}
                   ${LIBTIMER_SRC_TOP}
                 )
set(LIBPROCESS_SRC process_id.cc
                   process_role.cc
                   process_parent.cc
                   process_child.cc
                   process_handler.cc
                   process_signal.cc
                   process_signal_action.cc
                   process_signal_set.cc
                   process_signal_ctrl.cc
                   )

set(LIBPROCESS_TARGETS)

add_library(libprocess.so SHARED ${LIBPROCESS_SRC})
add_dependencies(libprocess.so ${DEPEND_PROJ_TARGETS})
target_include_directories(libprocess.so PRIVATE ${LIBPROCESS_HRD} ${DEPEND_INCLUDE_DIR})
set_target_properties(libprocess.so PROPERTIES OUTPUT_NAME "process" CLEAN_DIRECT_OUTPUT 1)
list(APPEND LIBPROCESS_TARGETS libprocess.so)

if(BUILD_STATIC_LIBS)
    add_library(libprocess.a STATIC ${LIBPROCESS_SRC})
    add_dependencies(libprocess.a ${DEPEND_PROJ_TARGETS})
    target_include_directories(libprocess.a PRIVATE ${LIBPROCESS_HRD} ${DEPEND_INCLUDE_DIR})
    set_target_properties(libprocess.a PROPERTIES OUTPUT_NAME "process" CLEAN_DIRECT_OUTPUT 1)
    list(APPEND LIBPROCESS_TARGETS libprocess.a)
endif()

#dependent project
set(DEPEND_TARGETS)
set(DEPEND_PROJ_TARGETS)
set(DEPEND_BINARY_DIR)
set(DEPEND_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include")
set(DEPEND_LIB_DIR "${CMAKE_BINARY_DIR}/lib")
set(DEPEND_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

#libfile
set(DEPEND_LIBFILE_PROJ_NAME libfile)
set(DEPEND_LIBFILE_TARGET libfile.so)
set(DEPEND_LIBFILE_BINARY_DIR "depend/src/${DEPEND_LIBFILE_PROJ_NAME}-build")
set(DEPEND_LIBFILE_CMAKE_ARGS)
list(APPEND DEPEND_LIBFILE_CMAKE_ARGS "-DBUILD_TEST=OFF")
list(APPEND DEPEND_LIBFILE_CMAKE_ARGS "-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}")
list(APPEND DEPEND_LIBFILE_CMAKE_ARGS "-DBUILD_DEPEND_LIBS=${BUILD_DEPEND_LIBS}")
list(APPEND DEPEND_LIBFILE_CMAKE_ARGS "-DOUT=${CMAKE_BINARY_DIR}")
ExternalProject_Add(${DEPEND_LIBFILE_PROJ_NAME} PREFIX ${DEPEND_PREFIX}
                    SOURCE_DIR ${LIBFILE_SRC_TOP}
                    BINARY_DIR ${DEPEND_LIBFILE_BINARY_DIR}
                    CONFIGURE_COMMAND ${CMAKE} ${DEPEND_LIBFILE_CMAKE_ARGS} ${LIBFILE_SRC_TOP}
                    BUILD_COMMAND $(MAKE) ${DEPEND_LIBFILE_TARGET}
                    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                    )
list(APPEND DEPEND_TARGETS ${DEPEND_LIBFILE_TARGET})
list(APPEND DEPEND_PROJ_TARGETS ${DEPEND_LIBFILE_PROJ_NAME})
list(APPEND DEPEND_BINARY_DIR "${CMAKE_BINARY_DIR}/${DEPEND_LIBFILE_BINARY_DIR}")

add_custom_target(lib)
if(BUILD_DEPEND_LIBS)
    add_dependencies(lib ${DEPEND_PROJ_TARGETS})
endif()
add_dependencies(lib ${LIBPROCESS_TARGETS})

INSTALL(FILES process_return.h
              process_id.h
        TYPE INCLUDE
        )

INSTALL(TARGETS ${LIBPROCESS_TARGETS}
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
       )

if(BUILD_DEPEND_LIBS AND DEPEND_PROJ_TARGETS)
    INSTALL(DIRECTORY "${CMAKE_BINARY_DIR}/include"
           TYPE INCLUDE
           )
endif()

#test
if(BUILD_TEST)
    add_subdirectory(test)
endif()

