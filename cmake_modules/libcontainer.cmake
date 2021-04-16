message("build libcontainer")

if (NOT DEFINED DEPEND_SRC)
    message(FATAL_ERROR "DEPEND_SRC not set.")
endif ()

set(libcontainer_DIR "${DEPEND_SRC}/install/cmake")
set(DEPEND_LIBCONTAINER_INSTALL_DIR "${DEPEND_SRC}/install")

find_package(libcontainer QUIET)
if (${libcontainer_FOUND})
    message("libcontainer already build")
    include_directories(${DEPEND_LIBCONTAINER_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBCONTAINER_INSTALL_DIR}/lib)
    list(APPEND DEPEND_INCLUDE_DIR "${DEPEND_LIBCONTAINER_INSTALL_DIR}/include")
    list(APPEND DEPEND_LIB_DIR "${DEPEND_LIBCONTAINER_INSTALL_DIR}/lib")
else ()
    include(ExternalProject)
    set(DEPEND_LIBCONTAINER_CMAKE_ARGS)
    list(APPEND DEPEND_LIBCONTAINER_CMAKE_ARGS "-DBUILD_UT=OFF")
    list(APPEND DEPEND_LIBCONTAINER_CMAKE_ARGS "-DBUILD_TEST=OFF")
    list(APPEND DEPEND_LIBCONTAINER_CMAKE_ARGS "-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}")
    list(APPEND DEPEND_LIBCONTAINER_CMAKE_ARGS "-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")
    list(APPEND DEPEND_LIBCONTAINER_CMAKE_ARGS "-DBUILD_DEPEND_LIBS=ON")
    list(APPEND DEPEND_LIBCONTAINER_CMAKE_ARGS "-DINTERNAL_AS_DEPEND_PROJ=ON")
    list(APPEND DEPEND_LIBCONTAINER_CMAKE_ARGS "-DDEPEND_SRC=${DEPEND_SRC}")
    list(APPEND DEPEND_LIBCONTAINER_CMAKE_ARGS "-DOUT=${DEPEND_SRC}/install")

    ExternalProject_Add(libcontainer PREFIX ${DEPEND_SRC}
                        SOURCE_DIR "${DEPEND_SRC}/libcontainer"
                        INSTALL_DIR "${DEPEND_SRC}/install"
                        DOWNLOAD_COMMAND ""
                        CONFIGURE_COMMAND ${CMAKE} ${DEPEND_LIBCONTAINER_CMAKE_ARGS} ${DEPEND_SRC}/libcontainer
                        BUILD_COMMAND $(MAKE)
                        INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                        )
    ExternalProject_Add_Step(libcontainer prepare_source
                             COMMENT "Prepare libcontainer source code"
                             COMMAND rm -rf ${DEPEND_SRC}/libcontainer
                             COMMAND ln -s ${LIBCONTAINER_SRC_TOP} ${DEPEND_SRC}/libcontainer)
    ExternalProject_Add_StepTargets(libcontainer prepare_source)

    list(APPEND DEPEND_PROJECTS libcontainer)
    include_directories(${DEPEND_LIBCONTAINER_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBCONTAINER_INSTALL_DIR}/lib)
    list(APPEND DEPEND_INCLUDE_DIR "${DEPEND_LIBCONTAINER_INSTALL_DIR}/include")
    list(APPEND DEPEND_LIB_DIR "${DEPEND_LIBCONTAINER_INSTALL_DIR}/lib")
endif ()
