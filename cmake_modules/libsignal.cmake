message("build libsignal")

if (NOT DEFINED DEPEND_SRC)
    message(FATAL_ERROR "DEPEND_SRC not set.")
endif ()

set(libsignal_DIR "${DEPEND_SRC}/install/cmake")
set(DEPEND_LIBSIGNAL_INSTALL_DIR "${DEPEND_SRC}/install")

find_package(libsignal QUIET)
if (${libsignal_FOUND})
    message("libsignal already build")
    include_directories(${DEPEND_LIBSIGNAL_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBSIGNAL_INSTALL_DIR}/lib)
else ()
    include(ExternalProject)
    set(DEPEND_LIBSIGNAL_CMAKE_ARGS)
    list(APPEND DEPEND_LIBSIGNAL_CMAKE_ARGS "-DBUILD_UT=OFF")
    list(APPEND DEPEND_LIBSIGNAL_CMAKE_ARGS "-DBUILD_TEST=OFF")
    list(APPEND DEPEND_LIBSIGNAL_CMAKE_ARGS "-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}")
    list(APPEND DEPEND_LIBSIGNAL_CMAKE_ARGS "-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")
    list(APPEND DEPEND_LIBSIGNAL_CMAKE_ARGS "-DBUILD_DEPEND_LIBS=ON")
    list(APPEND DEPEND_LIBSIGNAL_CMAKE_ARGS "-DINTERNAL_AS_DEPEND_PROJ=ON")
    list(APPEND DEPEND_LIBSIGNAL_CMAKE_ARGS "-DDEPEND_SRC=${DEPEND_SRC}")
    list(APPEND DEPEND_LIBSIGNAL_CMAKE_ARGS "-DOUT=${DEPEND_SRC}/install")

    ExternalProject_Add(libsignal PREFIX ${DEPEND_SRC}
                        SOURCE_DIR "${DEPEND_SRC}/libsignal"
                        INSTALL_DIR "${DEPEND_SRC}/install"
                        DOWNLOAD_COMMAND ""
                        CONFIGURE_COMMAND ${CMAKE} ${DEPEND_LIBSIGNAL_CMAKE_ARGS} ${DEPEND_SRC}/libsignal
                        BUILD_COMMAND $(MAKE)
                        INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                        )
    ExternalProject_Add_Step(libsignal prepare_source
                             COMMENT "Prepare libsignal source code"
                             COMMAND rm -rf ${DEPEND_SRC}/libsignal
                             COMMAND ln -s ${LIBSIGNAL_SRC_TOP} ${DEPEND_SRC}/libsignal)
    ExternalProject_Add_StepTargets(libsignal prepare_source)

    list(APPEND DEPEND_PROJECTS libsignal)
    include_directories(${DEPEND_LIBSIGNAL_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBSIGNAL_INSTALL_DIR}/lib)
endif ()
