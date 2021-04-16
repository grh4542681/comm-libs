message("build libio")

if (NOT DEFINED DEPEND_SRC)
    message(FATAL_ERROR "DEPEND_SRC not set.")
endif ()

set(libio_DIR "${DEPEND_SRC}/install/cmake")
set(DEPEND_LIBIO_INSTALL_DIR "${DEPEND_SRC}/install")

find_package(libio QUIET)
if (${libio_FOUND})
    message("libio already build")
    include_directories(${DEPEND_LIBIO_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBIO_INSTALL_DIR}/lib)
    list(APPEND DEPEND_INCLUDE_DIR "${DEPEND_LIBIO_INSTALL_DIR}/include")
    list(APPEND DEPEND_LIB_DIR "${DEPEND_LIBIO_INSTALL_DIR}/lib")
else ()
    include(ExternalProject)
    set(DEPEND_LIBIO_CMAKE_ARGS)
    list(APPEND DEPEND_LIBIO_CMAKE_ARGS "-DBUILD_UT=OFF")
    list(APPEND DEPEND_LIBIO_CMAKE_ARGS "-DBUILD_TEST=OFF")
    list(APPEND DEPEND_LIBIO_CMAKE_ARGS "-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}")
    list(APPEND DEPEND_LIBIO_CMAKE_ARGS "-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")
    list(APPEND DEPEND_LIBIO_CMAKE_ARGS "-DBUILD_DEPEND_LIBS=ON")
    list(APPEND DEPEND_LIBIO_CMAKE_ARGS "-DINTERNAL_AS_DEPEND_PROJ=ON")
    list(APPEND DEPEND_LIBIO_CMAKE_ARGS "-DDEPEND_SRC=${DEPEND_SRC}")
    list(APPEND DEPEND_LIBIO_CMAKE_ARGS "-DOUT=${DEPEND_SRC}/install")

    ExternalProject_Add(libio PREFIX ${DEPEND_SRC}
                        SOURCE_DIR "${DEPEND_SRC}/libio"
                        INSTALL_DIR "${DEPEND_SRC}/install"
                        DOWNLOAD_COMMAND ""
                        CONFIGURE_COMMAND ${CMAKE} ${DEPEND_LIBIO_CMAKE_ARGS} ${DEPEND_SRC}/libio
                        BUILD_COMMAND $(MAKE)
                        INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                        )
    ExternalProject_Add_Step(libio prepare_source
                             COMMENT "Prepare libio source code"
                             COMMAND rm -rf ${DEPEND_SRC}/libio
                             COMMAND ln -s ${LIBIO_SRC_TOP} ${DEPEND_SRC}/libio)
    ExternalProject_Add_StepTargets(libio prepare_source)

    list(APPEND DEPEND_PROJECTS libio)
    include_directories(${DEPEND_LIBIO_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBIO_INSTALL_DIR}/lib)
    list(APPEND DEPEND_INCLUDE_DIR "${DEPEND_LIBIO_INSTALL_DIR}/include")
    list(APPEND DEPEND_LIB_DIR "${DEPEND_LIBIO_INSTALL_DIR}/lib")
endif ()
