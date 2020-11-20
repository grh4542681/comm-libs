cmake_minimum_required(VERSION 3.16)                                                                                                                                                     
project(libcrypt VERSION 1.0.0
        DESCRIPTION "common crypt library")

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

    add_custom_target(depend-clean COMMAND rm -rf ${DEPEND_SRC})
endif ()

#project
INSTALL(FILES crypt_log.h
              crypt_return.h
              crypt_snowflash.h
        TYPE INCLUDE
        )

include(CMakePackageConfigHelpers)
set(LIBCRYPT_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(LIBCRYPT_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")

configure_package_config_file(cmake/libcryptConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/libcryptConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
  PATH_VARS LIBCRYPT_INCLUDE_DIR LIBCRYPT_LIBRARY_DIR)
#write_basic_package_version_file(
#  ${CMAKE_CURRENT_BINARY_DIR}/FooConfigVersion.cmake
#  VERSION 1.2.3
#  COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libcryptConfig.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)

#test
if(BUILD_TEST AND BUILD_DEPEND_LIBS)
    add_subdirectory(test)
endif()
