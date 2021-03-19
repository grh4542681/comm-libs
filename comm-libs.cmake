#complate arguments
set(CMAKE "/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake")
set(CMAKE_C_COMPILER "/home/ezgaoro/workspace/gcc-10.1.0/bin/gcc")
set(CMAKE_CXX_COMPILER "/home/ezgaoro/workspace/gcc-10.1.0/bin/g++")

set(COMPILE_WARNINGS "-Wall")
set(COMPILE_OPTIMIZATION "-Wl,--as-needed -O2")
set(COMPILE_DEBUG "-g -ggdb")

set(C_COMPILE_FLAGS "")
set(CXX_COMPILE_FLAGS "-std=c++20 -fPIC ${COMPILE_WARNINGS} ${COMPILE_OPTIMIZATION} ${COMPILE_DEBUG}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}")

#project arguments
set(COMM_LIBS_PROJ_TOP $ENV{REPOTOP})
if(NOT OUT)
    set(OUT "${COMM_LIBS_PROJ_TOP}/install")
endif()
set(CMAKE_INSTALL_PREFIX ${OUT})

#3pp
set(3PP_DIR "${COMM_LIBS_PROJ_TOP}/3pp")
set(3PP_BUILD_DIR "${3PP_DIR}/build")
set(3PP_INSTALL_DIR "${3PP_DIR}/install")
list(APPEND CMAKE_MODULE_PATH ${3PP_DIR}/cmake_modules)
set(3PP_GTEST_SRC_TOP "${3PP_DIR}/gtest")
set(3PP_RAPIDJSON_SRC_TOP "${3PP_DIR}/rapidjson")

#comm-libs
list(APPEND CMAKE_MODULE_PATH ${COMM_LIBS_PROJ_TOP}/cmake_modules)
set(LIBBASE_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libbase")
set(LIBMODEL_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libmodel")
set(LIBCRYPT_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libcrypt")
set(LIBCONTAINER_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libcontainer")
set(LIBLOG_SRC_TOP "${COMM_LIBS_PROJ_TOP}/liblog")
set(LIBUTIL_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libutil")
set(LIBIO_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libio")
set(LIBFILE_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libfile")
set(LIBTIMER_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libtimer")
set(LIBSIGNAL_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libsignal")
set(LIBPROCESS_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libprocess")
set(LIBTHREAD_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libthread")
