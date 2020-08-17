message("build libutil")

if (NOT DEFINED DEPEND_SRC)
    message(FATAL_ERROR "DEPEND_SRC not set.")
endif ()

set(libutil_DIR "${DEPEND_SRC}/install/cmake")
set(DEPEND_LIBUTIL_INSTALL_DIR "${DEPEND_SRC}/install")

find_package(libutil QUIET)
if (${libutil_FOUND})
    message("libutil already build")
    include_directories(${DEPEND_LIBUTIL_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBUTIL_INSTALL_DIR}/lib)
else ()
    include(ExternalProject)
    set(DEPEND_LIBUTIL_CMAKE_ARGS)
    list(APPEND DEPEND_LIBUTIL_CMAKE_ARGS "-DBUILD_TEST=OFF")
    list(APPEND DEPEND_LIBUTIL_CMAKE_ARGS "-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}")
    list(APPEND DEPEND_LIBUTIL_CMAKE_ARGS "-DBUILD_DEPEND_LIBS=ON")
    list(APPEND DEPEND_LIBUTIL_CMAKE_ARGS "-DINTERNAL_AS_DEPEND_PROJ=ON")
    list(APPEND DEPEND_LIBUTIL_CMAKE_ARGS "-DDEPEND_SRC=${DEPEND_SRC}")
    list(APPEND DEPEND_LIBUTIL_CMAKE_ARGS "-DOUT=${DEPEND_SRC}/install")

    ExternalProject_Add(libutil PREFIX ${DEPEND_SRC}
                        SOURCE_DIR "${DEPEND_SRC}/libutil"
                        INSTALL_DIR "${DEPEND_SRC}/install"
                        DOWNLOAD_COMMAND ""
                        CONFIGURE_COMMAND ${CMAKE} ${DEPEND_LIBUTIL_CMAKE_ARGS} ${DEPEND_SRC}/libutil
                        BUILD_COMMAND $(MAKE)
                        INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                        )
    ExternalProject_Add_Step(libutil prepare_source
                             COMMENT "Prepare libutil source code"
                             COMMAND rm -rf ${DEPEND_SRC}/libutil
                             COMMAND ln -s ${LIBUTIL_SRC_TOP} ${DEPEND_SRC}/libutil)
    ExternalProject_Add_StepTargets(libutil prepare_source)

    list(APPEND DEPEND_PROJECTS libutil)
    include_directories(${DEPEND_LIBUTIL_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBUTIL_INSTALL_DIR}/lib)
endif ()
