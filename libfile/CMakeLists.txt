cmake_minimum_required(VERSION 3.16)
project(libfile VERSION 1.0.0
        DESCRIPTION "common file library")

if(DEFINED ENV{REPOTOP})
    include($ENV{REPOTOP}/comm-libs.cmake)
else ()
    message(FATAL_ERROR "Env REPOTOP not set")
endif ()

set(PROJ_ROOT_PATH ${CMAKE_SOURCE_DIR})

option(BUILD_TEST "Build test program" ON)
option(BUILD_STATIC_LIBS "Build static Libraries" OFF)
option(BUILD_DEPEND_LIBS "Build dependent Libraries" ON)

option(INTERNAL_AS_DEPEND_PROJ "As dependent" OFF)

#dependent project
if (BUILD_DEPEND_LIBS STREQUAL ON)
    if (INTERNAL_AS_DEPEND_PROJ STREQUAL OFF)
        set(DEPEND_SRC ${CMAKE_BINARY_DIR}/depended)
    endif ()
    set(DEPEND_PROJECTS)

    include(liblog)
    include(libutil)

    add_custom_target(depend-clean COMMAND rm -rf ${DEPEND_SRC})
endif ()

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

include(CMakePackageConfigHelpers)
set(LIBFILE_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(LIBFILE_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")

configure_package_config_file(cmake/libfileConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/libfileConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
  PATH_VARS LIBFILE_INCLUDE_DIR LIBFILE_LIBRARY_DIR)
#write_basic_package_version_file(
#  ${CMAKE_CURRENT_BINARY_DIR}/FooConfigVersion.cmake
#  VERSION 1.2.3
#  COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libfileConfig.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)

if(BUILD_TEST AND BUILD_DEPEND_LIBS)
    add_subdirectory(test)
endif()
