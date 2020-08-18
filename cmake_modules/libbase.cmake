message("build libbase")

if (NOT DEFINED DEPEND_SRC)
    message(FATAL_ERROR "DEPEND_SRC not set.")
endif ()

set(libbase_DIR "${DEPEND_SRC}/install/cmake")
set(DEPEND_LIBBASE_INSTALL_DIR "${DEPEND_SRC}/install")

find_package(libbase QUIET)
if (${libbase_FOUND})
    message("libbase already build")
    include_directories(${DEPEND_LIBBASE_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBBASE_INSTALL_DIR}/lib)
else ()
    include(ExternalProject)
    set(DEPEND_LIBBASE_CMAKE_ARGS)
    list(APPEND DEPEND_LIBBASE_CMAKE_ARGS "-DBUILD_UT=OFF")
    list(APPEND DEPEND_LIBBASE_CMAKE_ARGS "-DBUILD_TEST=OFF")
    list(APPEND DEPEND_LIBBASE_CMAKE_ARGS "-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}")
    list(APPEND DEPEND_LIBBASE_CMAKE_ARGS "-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")
    list(APPEND DEPEND_LIBBASE_CMAKE_ARGS "-DBUILD_DEPEND_LIBS=ON")
    list(APPEND DEPEND_LIBBASE_CMAKE_ARGS "-DINTERNAL_AS_DEPEND_PROJ=ON")
    list(APPEND DEPEND_LIBBASE_CMAKE_ARGS "-DDEPEND_SRC=${DEPEND_SRC}")
    list(APPEND DEPEND_LIBBASE_CMAKE_ARGS "-DOUT=${DEPEND_SRC}/install")

    ExternalProject_Add(libbase PREFIX ${DEPEND_SRC}
                        SOURCE_DIR "${DEPEND_SRC}/libbase"
                        INSTALL_DIR "${DEPEND_SRC}/install"
                        DOWNLOAD_COMMAND ""
                        CONFIGURE_COMMAND ${CMAKE} ${DEPEND_LIBBASE_CMAKE_ARGS} ${DEPEND_SRC}/libbase
                        BUILD_COMMAND $(MAKE)
                        INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                        )
    ExternalProject_Add_Step(libbase prepare_source
                             COMMENT "Prepare libbase source code"
                             COMMAND rm -rf ${DEPEND_SRC}/libbase
                             COMMAND ln -s ${LIBBASE_SRC_TOP} ${DEPEND_SRC}/libbase)
    ExternalProject_Add_StepTargets(libbase prepare_source)

    list(APPEND DEPEND_PROJECTS libbase)
    include_directories(${DEPEND_LIBBASE_INSTALL_DIR}/include)
    link_directories(${DEPEND_LIBBASE_INSTALL_DIR}/lib)
endif ()
