cmake_minimum_required(VERSION 3.16)

project(comm-libs)

include(comm-libs.cmake)
include(ExternalProject)

option(BUILD_UT "Build unittest" ON)
option(BUILD_3PP "Build 3pp projects" ON)
option(BUILD_TEST "Build test program" ON)
#option(BUILD_STATIC_INFRA "Build static infra lib" ON)
option(BUILD_SHARED_INFRA "Build shared infra lib" ON)

add_custom_target(3pp)
if (BUILD_3PP STREQUAL ON)
    include(BuildGTest)
    include(BuildRapidjson)
endif ()
add_custom_target(3pp-clean COMMAND rm -rf ${3PP_BUILD_DIR}
                            COMMAND rm -rf ${3PP_INSTALL_DIR})

set(DEPEND_SRC ${CMAKE_BINARY_DIR}/libs)
set(BUILD_STATIC_LIBS ON)
include(libbase)
include(libutil)
include(liblog)
include(libfile)
include(libthread)
include(libprocess)
set(INFRA_DEPEND_PROJECTS libbase
                          libutil
                          liblog
                          libfile
                          libthread
                          libprocess)
set(INFRA_DEPEND_LIBS util
                      file
                      thread
                      process
                      )

file(TOUCH ${CMAKE_BINARY_DIR}/infra_dump.cc)
if(BUILD_SHARED_INFRA)
    add_library(libinfra.so SHARED ${CMAKE_BINARY_DIR}/infra_dump.cc)
    target_link_directories(libinfra.so PRIVATE ${DEPEND_SRC}/install/lib)
    set_target_properties(libinfra.so PROPERTIES OUTPUT_NAME "infra" CLEAN_DIRECT_OUTPUT 1)
    target_link_libraries(libinfra.so -Wl,--whole-archive ${INFRA_DEPEND_LIBS} -Wl,--no-whole-archive)
    add_dependencies(libinfra.so ${INFRA_DEPEND_PROJECTS})
endif()

#if(BUILD_STATIC_INFRA)
#    add_library(libinfra.a STATIC ${CMAKE_BINARY_DIR}/infra_dump.cc)
#    target_link_directories(libinfra.a PRIVATE ${DEPEND_SRC}/install/lib)
#    set_target_properties(libinfra.a PROPERTIES OUTPUT_NAME "infra" CLEAN_DIRECT_OUTPUT 1)
#    target_link_libraries(libinfra.a -Wl,--whole-archive ${INFRA_DEPEND_LIBS} -Wl,--no-whole-archive)
#    add_dependencies(libinfra.a ${INFRA_DEPEND_PROJECTS})
#endif()

add_custom_target(libs-clean COMMAND rm -rf ${DEPEND_SRC})

install(DIRECTORY ${DEPEND_SRC}/install/ DESTINATION ${CMAKE_INSTALL_PREFIX})
