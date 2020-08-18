message("build libprocess")

if (NOT DEFINED DEPEND_SRC)
    message(FATAL_ERROR "DEPEND_SRC not set.")
endif ()

set(libprocess_DIR "${DEPEND_SRC}/install/cmake")
set(DEPEND_LIBPROCESS_INSTALL_DIR "${DEPEND_SRC}/install")

find_package(libprocess QUIET)
if (${libprocess_FOUND})
    message("libprocess already build")
    include_directories(${DEPEND_LIBPROCESS_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBPROCESS_INSTALL_DIR}/lib)
else ()
    include(ExternalProject)
    set(DEPEND_LIBPROCESS_CMAKE_ARGS)
    list(APPEND DEPEND_LIBPROCESS_CMAKE_ARGS "-DBUILD_UT=OFF")
    list(APPEND DEPEND_LIBPROCESS_CMAKE_ARGS "-DBUILD_TEST=OFF")
    list(APPEND DEPEND_LIBPROCESS_CMAKE_ARGS "-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}")
    list(APPEND DEPEND_LIBPROCESS_CMAKE_ARGS "-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")
    list(APPEND DEPEND_LIBPROCESS_CMAKE_ARGS "-DBUILD_DEPEND_LIBS=ON")
    list(APPEND DEPEND_LIBPROCESS_CMAKE_ARGS "-DINTERNAL_AS_DEPEND_PROJ=ON")
    list(APPEND DEPEND_LIBPROCESS_CMAKE_ARGS "-DDEPEND_SRC=${DEPEND_SRC}")
    list(APPEND DEPEND_LIBPROCESS_CMAKE_ARGS "-DOUT=${DEPEND_SRC}/install")

    ExternalProject_Add(libprocess PREFIX ${DEPEND_SRC}
                        SOURCE_DIR "${DEPEND_SRC}/libprocess"
                        INSTALL_DIR "${DEPEND_SRC}/install"
                        DOWNLOAD_COMMAND ""
                        CONFIGURE_COMMAND ${CMAKE} ${DEPEND_LIBPROCESS_CMAKE_ARGS} ${DEPEND_SRC}/libprocess
                        BUILD_COMMAND $(MAKE)
                        INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                        )
    ExternalProject_Add_Step(libprocess prepare_source
                             COMMENT "Prepare libprocess source code"
                             COMMAND rm -rf ${DEPEND_SRC}/libprocess
                             COMMAND ln -s ${LIBPROCESS_SRC_TOP} ${DEPEND_SRC}/libprocess)
    ExternalProject_Add_StepTargets(libprocess prepare_source)

    list(APPEND DEPEND_PROJECTS libprocess)
    include_directories(${DEPEND_LIBPROCESS_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBPROCESS_INSTALL_DIR}/lib)
endif ()
