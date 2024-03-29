cmake_minimum_required(VERSION 3.16)
project(libprocess VERSION 1.0.0
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

    include(libfile)
    include(libsignal)
    include(libthread)

    add_custom_target(depend-clean COMMAND rm -rf ${DEPEND_SRC})
endif ()

#project
set(LIBPROCESS_HRD ${PROJ_ROOT_PATH}
                   ${LIBBASE_SRC_TOP}
                   ${LIBCONTAINER_SRC_TOP}
                   ${LIBLOG_SRC_TOP}
                   ${LIBIO_SRC_TOP}
                   ${LIBFILE_SRC_TOP}
                   ${LIBTIMER_SRC_TOP}
                   ${LIBTHREAD_SRC_TOP}
                 )
set(LIBPROCESS_SRC process_id.cc
                   process_role.cc
                   process_relation.cc
                   process_parent.cc
                   process_child.cc
                   process_handler.cc
                   )

set(LIBPROCESS_TARGETS)

if(BUILD_SHARED_LIBS)
    add_library(libprocess.so SHARED ${LIBPROCESS_SRC})
    list(LENGTH DEPEND_PROJECTS DEPEND_PROJECTS_NUM)
    if (${DEPEND_PROJECTS_NUM})
        add_dependencies(libprocess.so ${DEPEND_PROJECTS})
    endif()
    target_include_directories(libprocess.so PRIVATE ${LIBPROCESS_HRD} ${DEPEND_INCLUDE_DIR})
    target_link_directories(libprocess.so PRIVATE ${CMAKE_BINARY_DIR} ${DEPEND_LIB_DIR})
    target_link_libraries(libprocess.so file signal thread)
    set_target_properties(libprocess.so PROPERTIES OUTPUT_NAME "process" CLEAN_DIRECT_OUTPUT 1)
    list(APPEND LIBPROCESS_TARGETS libprocess.so)
endif()

if(BUILD_STATIC_LIBS)
    add_library(libprocess.a STATIC ${LIBPROCESS_SRC})
    list(LENGTH DEPEND_PROJECTS DEPEND_PROJECTS_NUM)
    if (${DEPEND_PROJECTS_NUM})
        add_dependencies(libprocess.a ${DEPEND_PROJECTS})
    endif()
    target_include_directories(libprocess.a PRIVATE ${LIBPROCESS_HRD} ${DEPEND_INCLUDE_DIR})
    target_link_directories(libprocess.a PRIVATE ${CMAKE_BINARY_DIR} ${DEPEND_LIB_DIR})
    target_link_libraries(libprocess.a file signal thread)
    set_target_properties(libprocess.a PROPERTIES OUTPUT_NAME "process" CLEAN_DIRECT_OUTPUT 1)
    list(APPEND LIBPROCESS_TARGETS libprocess.a)
endif()

add_custom_target(lib)
add_dependencies(lib ${LIBPROCESS_TARGETS})

INSTALL(FILES process_return.h
              process_id.h
        TYPE INCLUDE
        )

INSTALL(TARGETS ${LIBPROCESS_TARGETS}
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
       )

include(CMakePackageConfigHelpers)
set(LIBPROCESS_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(LIBPROCESS_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")

configure_package_config_file(cmake/libprocessConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/libprocessConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
  PATH_VARS LIBPROCESS_INCLUDE_DIR LIBPROCESS_LIBRARY_DIR)
#write_basic_package_version_file(
#  ${CMAKE_CURRENT_BINARY_DIR}/FooConfigVersion.cmake
#  VERSION 1.2.3
#  COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libprocessConfig.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)

#test
if(BUILD_TEST AND BUILD_DEPEND_LIBS)
    add_subdirectory(test)
endif()

#ut
if(BUILD_UT)
    add_subdirectory(ut)
endif()
