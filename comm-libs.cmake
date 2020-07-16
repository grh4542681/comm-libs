#complate arguments
set(CMAKE "/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake")
set(CMAKE_C_COMPILER "/home/ezgaoro/workspace/gcc-10.1.0/bin/gcc")
set(CMAKE_CXX_COMPILER "/home/ezgaoro/workspace/gcc-10.1.0/bin/g++")

set(COMPILE_WARNINGS "-Wall")
set(COMPILE_OPTIMIZATION "-Wl,--as-needed -O2")
set(COMPILE_DEBUG "-g -ggdb")

set(C_COMPILE_FLAGS "")
set(CXX_COMPILE_FLAGS "-std=c++20 ${COMPILE_WARNINGS} ${COMPILE_OPTIMIZATION} ${COMPILE_DEBUG}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}")

#project arguments
set(COMM_LIBS_PROJ_TOP "/home/ezgaoro/workspace/comm-libs")
if(NOT OUT)
    set(OUT "${COMM_LIBS_PROJ_TOP}/output")
endif()
set(CMAKE_INSTALL_PREFIX ${OUT})
set(LIBBASE_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libbase")
set(LIBUTIL_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libutil")
set(LIBIO_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libio")
set(LIBFILE_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libfile")
set(LIBTIMER_SRC_TOP "${COMM_LIBS_PROJ_TOP}/libtimer")
set(LIBLOG_SRC_TOP "${COMM_LIBS_PROJ_TOP}/liblog")
