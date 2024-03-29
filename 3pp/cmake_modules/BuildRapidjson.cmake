message("Build rapidjson 3pp")

include(ExternalProject)

set(RAPIDJSON_CMAKE_ARGS)
list(APPEND RAPIDJSON_CMAKE_ARGS "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
list(APPEND RAPIDJSON_CMAKE_ARGS "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
list(APPEND RAPIDJSON_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${3PP_INSTALL_DIR}")
list(APPEND RAPIDJSON_CMAKE_ARGS "-DRAPIDJSON_BUILD_DOC=OFF")
list(APPEND RAPIDJSON_CMAKE_ARGS "-DRAPIDJSON_BUILD_EXAMPLES=OFF")
list(APPEND RAPIDJSON_CMAKE_ARGS "-DRAPIDJSON_BUILD_TESTS=OFF")

ExternalProject_Add(rapidjson
                    PREFIX ${3PP_BUILD_DIR}
                    SOURCE_DIR ${3PP_RAPIDJSON_SRC_TOP}
                    INSTALL_DIR ${3PP_INSTALL_DIR}
                    CONFIGURE_COMMAND ${CMAKE} ${RAPIDJSON_CMAKE_ARGS} ${3PP_RAPIDJSON_SRC_TOP}
                    BUILD_COMMAND $(MAKE)
                    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
                    )

add_dependencies(3pp rapidjson)

message("Build rapidjson 3pp done.")
