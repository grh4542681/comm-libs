message("build depend libfile")

if (not DEPEND_SRC)
    message("DEPEND_SRC not set.")
endif ()

find_package(libfile_depend)
if (${LIBFILE_FOUND})
    message("depend libfile already build")
else ()
    set(DEPEND_LIBFILE_CMAKE_ARGS)
    list(APPEND DEPEND_LIBFILE_CMAKE_ARGS "-DBUILD_TEST=OFF")
    list(APPEND DEPEND_LIBFILE_CMAKE_ARGS "-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}")
    list(APPEND DEPEND_LIBFILE_CMAKE_ARGS "-DBUILD_DEPEND_LIBS=${BUILD_DEPEND_LIBS}")
    list(APPEND DEPEND_LIBFILE_CMAKE_ARGS "-DOUT=${CMAKE_BINARY_DIR}")
    ExternalProject_Add(depend_libfile
                        SOURCE_DIR ${DEPEND_SRC}
                        DOWNLOAD_COMMAND "cp -r ${LIBFILE_SRC_TOP} ${DEPEND_SRC}"
                        UPGRADE_COMMAND "cp -r ${LIBFILE_SRC_TOP} ${DEPEND_SRC}"
                        UPGRADE_COMMAND ""
                        CONFIGURE_COMMAND ${CMAKE} ${DEPEND_LIBFILE_CMAKE_ARGS} ${LIBFILE_SRC_TOP}
                        BUILD_COMMAND $(MAKE)
                        INSTALL_COMMAND ""
                        )

    ExternalProject_Get_property(depend_libfile SOURCE_DIR)
    set(DEPEND_LIBFILE_SOURCE_DIR ${SOURCE_DIR})
endif ()
