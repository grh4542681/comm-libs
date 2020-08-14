message("build libfile")

if (NOT DEFINED DEPEND_SRC)
    message(FATAL_ERROR "DEPEND_SRC not set.")
endif ()

set(libfile_DIR "${DEPEND_SRC}/install/cmake")
set(DEPEND_LIBFILE_INSTALL_DIR "${DEPEND_SRC}/install")

find_package(libfile QUIET)
if (${libfile_FOUND})
    message("libfile already build")
    include_directories(${DEPEND_LIBFILE_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBFILE_INSTALL_DIR}/lib)
else ()
    include(ExternalProject)
    set(DEPEND_LIBFILE_CMAKE_ARGS)
    list(APPEND DEPEND_LIBFILE_CMAKE_ARGS "-DBUILD_TEST=OFF")
    list(APPEND DEPEND_LIBFILE_CMAKE_ARGS "-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}")
    list(APPEND DEPEND_LIBFILE_CMAKE_ARGS "-DBUILD_DEPEND_LIBS=${BUILD_DEPEND_LIBS}")
    list(APPEND DEPEND_LIBFILE_CMAKE_ARGS "-DINTERNAL_AS_DEPEND_PROJ=ON")
    list(APPEND DEPEND_LIBFILE_CMAKE_ARGS "-DDEPEND_SRC=${DEPEND_SRC}")
    list(APPEND DEPEND_LIBFILE_CMAKE_ARGS "-DOUT=${DEPEND_SRC}/install")

    ExternalProject_Add(libfile PREFIX ${DEPEND_SRC}
                        SOURCE_DIR "${DEPEND_SRC}/libfile"
                        INSTALL_DIR "${DEPEND_SRC}/install"
                        DOWNLOAD_COMMAND ""
                        CONFIGURE_COMMAND ${CMAKE} ${DEPEND_LIBFILE_CMAKE_ARGS} ${DEPEND_SRC}/libfile
                        BUILD_COMMAND $(MAKE)
                        INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                        )
    ExternalProject_Add_Step(libfile prepare_source
                             COMMENT "Prepare libfile source code"
                             COMMAND rm -rf ${DEPEND_SRC}/libfile
                             COMMAND ln -s ${LIBFILE_SRC_TOP} ${DEPEND_SRC}/libfile)
    ExternalProject_Add_StepTargets(libfile prepare_source)

    list(APPEND DEPEND_PROJECTS libfile)
    include_directories(${DEPEND_LIBFILE_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBFILE_INSTALL_DIR}/lib)
endif ()
