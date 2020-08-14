message("Build Protobuf 3pp")

include(ExternalProject)

set(PB_DIR ${FRAM_3PP_PATH}/protobuf)

set(PB_CMAKE_ARGS)
list(APPEND PB_CMAKE_ARGS "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
list(APPEND PB_CMAKE_ARGS "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
list(APPEND PB_CMAKE_ARGS "-DLIBRARY_OUTPUT_PATH=${LIBRARY_OUTPUT_PATH}")
list(APPEND PB_CMAKE_ARGS "-DEXECUTABLE_OUTPUT_PATH=${EXECUTABLE_OUTPUT_PATH}")

list(APPEND PB_CMAKE_ARGS "-Dprotobuf_BUILD_TESTS=OFF")
list(APPEND PB_CMAKE_ARGS "-DBUILD_SHARED_LIBS=ON")
list(APPEND PB_CMAKE_ARGS "-Dprotobuf_BUILD_PROTOC_BINARIES=ON")

ExternalProject_Add(protobuf
                    PREFIX ${FRAM_ROOT_PATH}/build
                    SOURCE_DIR ${PB_DIR}
                    CONFIGURE_COMMAND ${CMAKE} ${PB_CMAKE_ARGS} ${PB_DIR}/cmake
                    BUILD_COMMAND $(MAKE)
                    INSTALL_COMMAND ""
                    )

set(3PP_PROTOBUF_HRD ${PB_DIR}/src)

set(3PP_PROTOBUF_CPP_GEN "${FRAM_BIN_PATH}/protoc")
set(3PP_PROTOBUF_CPP_INCLUDE "-I")
set(3PP_PROTOBUF_CPP_OUTPUT "--cpp_out")
set(3PP_PROTOBUF_LIBS libprotobuf.so libprotoc.so)

message("Build Protobuf done.")
