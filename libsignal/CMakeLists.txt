cmake_minimum_required(VERSION 3.16)
project(libsignal VERSION 1.0.0
        DESCRIPTION "common file library")

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

    include(libbase)

    add_custom_target(depend-clean COMMAND rm -rf ${DEPEND_SRC})
endif ()

#project
set(LIBSIGNAL_HRD ${PROJ_ROOT_PATH}
                   ${LIBBASE_SRC_TOP}
                   ${LIBLOG_SRC_TOP}
                   ${LIBTIMER_SRC_TOP}
                 )
set(LIBSIGNAL_SRC signal_id.cc
                  signal_set.cc
                  signal_action.cc
                  signal_process.cc
                  signal_thread.cc
                  )

set(LIBSIGNAL_TARGETS)

if(BUILD_SHARED_LIBS)
    add_library(libsignal.so SHARED ${LIBSIGNAL_SRC})
    target_include_directories(libsignal.so PRIVATE ${LIBSIGNAL_HRD} ${DEPEND_INCLUDE_DIR})
    set_target_properties(libsignal.so PROPERTIES OUTPUT_NAME "signal" CLEAN_DIRECT_OUTPUT 1)
    list(APPEND LIBSIGNAL_TARGETS libsignal.so)
endif()

if(BUILD_STATIC_LIBS)
    add_library(libsignal.a STATIC ${LIBSIGNAL_SRC})
    target_include_directories(libsignal.a PRIVATE ${LIBSIGNAL_HRD} ${DEPEND_INCLUDE_DIR})
    set_target_properties(libsignal.a PROPERTIES OUTPUT_NAME "signal" CLEAN_DIRECT_OUTPUT 1)
    list(APPEND LIBSIGNAL_TARGETS libsignal.a)
endif()

add_custom_target(lib)
add_dependencies(lib ${LIBSIGNAL_TARGETS})

INSTALL(FILES signal_return.h
              signal_id.h
              signal_set.h
              signal_action.h
              signal_process.h
              signal_thread.h
              signal_mask_type.h
        TYPE INCLUDE
        )

INSTALL(TARGETS ${LIBSIGNAL_TARGETS}
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
       )

include(CMakePackageConfigHelpers)
set(LIBSIGNAL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(LIBSIGNAL_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")

configure_package_config_file(cmake/libsignalConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/libsignalConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
  PATH_VARS LIBSIGNAL_INCLUDE_DIR LIBSIGNAL_LIBRARY_DIR)
#write_basic_package_version_file(
#  ${CMAKE_CURRENT_BINARY_DIR}/FooConfigVersion.cmake
#  VERSION 1.2.3
#  COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libsignalConfig.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)

#test
if(BUILD_TEST AND BUILD_DEPEND_LIBS)
    add_subdirectory(test)
endif()

#ut
if(BUILD_UT)
    add_subdirectory(ut)
endif()
