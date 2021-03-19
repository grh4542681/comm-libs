message("build libthread")

if (NOT DEFINED DEPEND_SRC)
    message(FATAL_ERROR "DEPEND_SRC not set.")
endif ()

set(libthread_DIR "${DEPEND_SRC}/install/cmake")
set(DEPEND_LIBTHREAD_INSTALL_DIR "${DEPEND_SRC}/install")

find_package(libthread QUIET)
if (${libthread_FOUND})
    message("libthread already build")
    include_directories(${DEPEND_LIBTHREAD_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBTHREAD_INSTALL_DIR}/lib)
else ()
    include(ExternalProject)
    set(DEPEND_LIBTHREAD_CMAKE_ARGS)
    list(APPEND DEPEND_LIBTHREAD_CMAKE_ARGS "-DBUILD_UT=OFF")
    list(APPEND DEPEND_LIBTHREAD_CMAKE_ARGS "-DBUILD_TEST=OFF")
    list(APPEND DEPEND_LIBTHREAD_CMAKE_ARGS "-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}")
    list(APPEND DEPEND_LIBTHREAD_CMAKE_ARGS "-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")
    list(APPEND DEPEND_LIBTHREAD_CMAKE_ARGS "-DBUILD_DEPEND_LIBS=ON")
    list(APPEND DEPEND_LIBTHREAD_CMAKE_ARGS "-DINTERNAL_AS_DEPEND_PROJ=ON")
    list(APPEND DEPEND_LIBTHREAD_CMAKE_ARGS "-DDEPEND_SRC=${DEPEND_SRC}")
    list(APPEND DEPEND_LIBTHREAD_CMAKE_ARGS "-DOUT=${DEPEND_SRC}/install")

    ExternalProject_Add(libthread PREFIX ${DEPEND_SRC}
                        SOURCE_DIR "${DEPEND_SRC}/libthread"
                        INSTALL_DIR "${DEPEND_SRC}/install"
                        DOWNLOAD_COMMAND ""
                        CONFIGURE_COMMAND ${CMAKE} ${DEPEND_LIBTHREAD_CMAKE_ARGS} ${DEPEND_SRC}/libthread
                        BUILD_COMMAND $(MAKE)
                        INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                        )
    ExternalProject_Add_Step(libthread prepare_source
                             COMMENT "Prepare libthread source code"
                             COMMAND rm -rf ${DEPEND_SRC}/libthread
                             COMMAND ln -s ${LIBTHREAD_SRC_TOP} ${DEPEND_SRC}/libthread)
    ExternalProject_Add_StepTargets(libthread prepare_source)

    list(APPEND DEPEND_PROJECTS libthread)
    include_directories(${DEPEND_LIBTHREAD_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBTHREAD_INSTALL_DIR}/lib)
endif ()
