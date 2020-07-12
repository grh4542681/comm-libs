set(LIBGRHSOCKET_HRD "${PLAT_INFRA_PATH}/return"
                     "${PLAT_INFRA_PATH}/timer"
                     "${PLAT_INFRA_PATH}/base"
                     "${PLAT_INFRA_PATH}/mempool"
                     "${PLAT_INFRA_PATH}/report"
                     "${PLAT_INFRA_PATH}/io"
                     "${PLAT_INFRA_PATH}/file"
                     )
set(LIBGRHSOCKET_SRC sock_return.cc
                     sock_address.cc
                     sock_fd.cc
                     sock_server.cc
                     sock_client.cc
                     )
add_library(GRHsocket_SO SHARED ${LIBGRHSOCKET_SRC})
target_include_directories(GRHsocket_SO PRIVATE ${LIBGRHSOCKET_HRD})
target_link_libraries(GRHsocket_SO pthread libgmp.so)
set_target_properties(GRHsocket_SO PROPERTIES OUTPUT_NAME "gsocket" CLEAN_DIRECT_OUTPUT 1)

if(BUILD_STATIC_LIBS)
    add_library(GRHsocket_A STATIC ${LIBGRHSOCKET_SRC})
    target_include_directories(GRHsocket_A PRIVATE ${LIBGRHSOCKET_HRD})
    target_link_libraries(GRHsocket_A pthread libgmp.so)
    set_target_properties(GRHsocket_A PROPERTIES OUTPUT_NAME "gsocket" CLEAN_DIRECT_OUTPUT 1)
endif()
