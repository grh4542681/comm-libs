cmake_minimum_required(VERSION 3.16)
project(libthread VERSION 1.0.0
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

    include(libsignal)

    add_custom_target(depend-clean COMMAND rm -rf ${DEPEND_SRC})
endif ()

#project
set(LIBTHREAD_HRD ${PROJ_ROOT_PATH}
                   ${LIBBASE_SRC_TOP}
                   ${LIBCONTAINER_SRC_TOP}
                   ${LIBLOG_SRC_TOP}
                   ${LIBIO_SRC_TOP}
                   ${LIBFILE_SRC_TOP}
                   ${LIBTIMER_SRC_TOP}
                 )
set(LIBTHREAD_SRC thread_id.cc
                  thread_role.cc
                  thread_mutex.cc
                  thread_rwlock.cc
                  thread_handler.cc
                  )

set(LIBTHREAD_TARGETS)

if(BUILD_SHARED_LIBS)
    add_library(libthread.so SHARED ${LIBTHREAD_SRC})
    target_include_directories(libthread.so PRIVATE ${LIBTHREAD_HRD} ${DEPEND_INCLUDE_DIR})
    set_target_properties(libthread.so PROPERTIES OUTPUT_NAME "thread" CLEAN_DIRECT_OUTPUT 1)
    list(APPEND LIBTHREAD_TARGETS libthread.so)
endif()

if(BUILD_STATIC_LIBS)
    add_library(libthread.a STATIC ${LIBTHREAD_SRC})
    target_include_directories(libthread.a PRIVATE ${LIBTHREAD_HRD} ${DEPEND_INCLUDE_DIR})
    set_target_properties(libthread.a PROPERTIES OUTPUT_NAME "thread" CLEAN_DIRECT_OUTPUT 1)
    list(APPEND LIBTHREAD_TARGETS libthread.a)
endif()

add_custom_target(lib)
add_dependencies(lib ${LIBTHREAD_TARGETS})

INSTALL(FILES thread_return.h
              thread_id.h
        TYPE INCLUDE
        )

INSTALL(TARGETS ${LIBTHREAD_TARGETS}
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
       )

include(CMakePackageConfigHelpers)
set(LIBTHREAD_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(LIBTHREAD_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")

configure_package_config_file(cmake/libthreadConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/libthreadConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
  PATH_VARS LIBTHREAD_INCLUDE_DIR LIBTHREAD_LIBRARY_DIR)
#write_basic_package_version_file(
#  ${CMAKE_CURRENT_BINARY_DIR}/FooConfigVersion.cmake
#  VERSION 1.2.3
#  COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libthreadConfig.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)

#test
if(BUILD_TEST AND BUILD_DEPEND_LIBS)
    add_subdirectory(test)
endif()

#ut
if(BUILD_UT)
    add_subdirectory(ut)
endif()
