message("Build GTest 3pp")

include(ExternalProject)

set(GTEST_CMAKE_ARGS)
list(APPEND GTEST_CMAKE_ARGS "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
list(APPEND GTEST_CMAKE_ARGS "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
list(APPEND GTEST_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${3PP_INSTALL_DIR}")

list(APPEND GTEST_CMAKE_ARGS "-DBUILD_SHARED_LIBS=ON")

ExternalProject_Add(gtest
                    PREFIX ${3PP_BUILD_DIR}
                    SOURCE_DIR ${3PP_GTEST_SRC_TOP}
                    INSTALL_DIR ${3PP_INSTALL_DIR}
                    CONFIGURE_COMMAND ${CMAKE} ${GTEST_CMAKE_ARGS} ${3PP_GTEST_SRC_TOP}
                    BUILD_COMMAND $(MAKE)
                    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                    )

add_dependencies(3pp gtest)

message("Build GTest 3pp done.")
