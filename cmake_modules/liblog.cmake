message("build liblog")

if (NOT DEFINED DEPEND_SRC)
    message(FATAL_ERROR "DEPEND_SRC not set.")
endif ()

set(liblog_DIR "${DEPEND_SRC}/install/cmake")
set(DEPEND_LIBLOG_INSTALL_DIR "${DEPEND_SRC}/install")

find_package(liblog QUIET)
if (${liblog_FOUND})
    message("liblog already build")
    include_directories(${DEPEND_LIBLOG_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBLOG_INSTALL_DIR}/lib)
else ()
    include(ExternalProject)
    set(DEPEND_LIBLOG_CMAKE_ARGS)
    list(APPEND DEPEND_LIBLOG_CMAKE_ARGS "-DBUILD_UT=OFF")
    list(APPEND DEPEND_LIBLOG_CMAKE_ARGS "-DBUILD_TEST=OFF")
    list(APPEND DEPEND_LIBLOG_CMAKE_ARGS "-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}")
    list(APPEND DEPEND_LIBLOG_CMAKE_ARGS "-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")
    list(APPEND DEPEND_LIBLOG_CMAKE_ARGS "-DBUILD_DEPEND_LIBS=ON")
    list(APPEND DEPEND_LIBLOG_CMAKE_ARGS "-DINTERNAL_AS_DEPEND_PROJ=ON")
    list(APPEND DEPEND_LIBLOG_CMAKE_ARGS "-DDEPEND_SRC=${DEPEND_SRC}")
    list(APPEND DEPEND_LIBLOG_CMAKE_ARGS "-DOUT=${DEPEND_SRC}/install")

    ExternalProject_Add(liblog PREFIX ${DEPEND_SRC}
                        SOURCE_DIR "${DEPEND_SRC}/liblog"
                        INSTALL_DIR "${DEPEND_SRC}/install"
                        DOWNLOAD_COMMAND ""
                        CONFIGURE_COMMAND ${CMAKE} ${DEPEND_LIBLOG_CMAKE_ARGS} ${DEPEND_SRC}/liblog
                        BUILD_COMMAND $(MAKE)
                        INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                        )
    ExternalProject_Add_Step(liblog prepare_source
                             COMMENT "Prepare liblog source code"
                             COMMAND rm -rf ${DEPEND_SRC}/liblog
                             COMMAND ln -s ${LIBLOG_SRC_TOP} ${DEPEND_SRC}/liblog)
    ExternalProject_Add_StepTargets(liblog prepare_source)

    list(APPEND DEPEND_PROJECTS liblog)
    include_directories(${DEPEND_LIBLOG_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBLOG_INSTALL_DIR}/lib)
endif ()
