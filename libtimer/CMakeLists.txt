cmake_minimum_required(VERSION 3.16)
project(libtimer VERSION 1.0.0
        DESCRIPTION "common timer library")

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

    include(liblog)
    include(libutil)
    include(libio)
    include(libcontainer)
    include(libthread)

    add_custom_target(depend-clean COMMAND rm -rf ${DEPEND_SRC})
endif ()

#project
set(LIBTIMER_HRD ${PROJ_ROOT_PATH}
                 ${LIBBASE_SRC_TOP}
                 ${LIBLOG_SRC_TOP}
                 ${LIBUTIL_SRC_TOP}
                 ${LIBCONTAINER_SRC_TOP}
                 ${LIBSIGNAL_SRC_TOP}
                 ${LIBIO_SRC_TOP}
                 ${LIBTHREAD_SRC_TOP}
                 )
set(LIBTIMER_SRC timer_fd.cc
                 timer_schedue.cc
                 timer_schedue_cronrule.cc
                 timer_clock_pump_handler.cc
                 )

set(LIBTIMER_TARGETS)
if(BUILD_SHARED_LIBS)
    add_library(libtimer.so SHARED ${LIBTIMER_SRC})
    list(LENGTH DEPEND_PROJECTS DEPEND_PROJECTS_NUM)
    if (${DEPEND_PROJECTS_NUM})
        add_dependencies(libtimer.so ${DEPEND_PROJECTS})
    endif()
    target_include_directories(libtimer.so PRIVATE ${LIBTIMER_HRD} ${DEPEND_INCLUDE_DIR})
    target_link_directories(libtimer.so PRIVATE ${CMAKE_BINARY_DIR} ${DEPEND_LIB_DIR})
    target_link_libraries(libtimer.so util io thread)
    set_target_properties(libtimer.so PROPERTIES OUTPUT_NAME "timer" CLEAN_DIRECT_OUTPUT 1)
    list(APPEND LIBTIMER_TARGETS libtimer.so)
endif()

if(BUILD_STATIC_LIBS)
    add_library(libtimer.a STATIC ${LIBTIMER_SRC})
    list(LENGTH DEPEND_PROJECTS DEPEND_PROJECTS_NUM)
    if (${DEPEND_PROJECTS_NUM})
        add_dependencies(libtimer.a ${DEPEND_PROJECTS})
    endif()
    target_include_directories(libtimer.a PRIVATE ${LIBTIMER_HRD} ${DEPEND_INCLUDE_DIR})
    target_link_directories(libtimer.a PRIVATE ${CMAKE_BINARY_DIR} ${DEPEND_LIB_DIR})
    target_link_libraries(libtimer.a util io thread)
    set_target_properties(libtimer.a PROPERTIES OUTPUT_NAME "timer" CLEAN_DIRECT_OUTPUT 1)
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

include(CMakePackageConfigHelpers)
set(LIBTIMER_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(LIBTIMER_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")

configure_package_config_file(cmake/libtimerConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/libtimerConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
  PATH_VARS LIBTIMER_INCLUDE_DIR LIBTIMER_LIBRARY_DIR)
#write_basic_package_version_file(
#  ${CMAKE_CURRENT_BINARY_DIR}/FooConfigVersion.cmake
#  VERSION 1.2.3
#  COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libtimerConfig.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)

#test
if(BUILD_TEST AND BUILD_DEPEND_LIBS)
    add_subdirectory(test)
endif()

#ut
if(BUILD_UT)
    add_subdirectory(ut)
endif()
