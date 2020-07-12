#SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${FRAM_BIN_PATH}/test)

set(SOCKET_TEST_HRD "${PLAT_INFRA_PATH}/return"
                    "${PLAT_INFRA_PATH}/base"
                    "${PLAT_INFRA_PATH}/socket"
                    "${PLAT_INFRA_PATH}/timer"
                    "${PLAT_INFRA_PATH}/mempool"
                    "${PLAT_INFRA_PATH}/report"
                    "${PLAT_INFRA_PATH}/io"
                    )
add_executable(sock_tcp_c sock_tcp_c.cc)
add_executable(sock_tcp_s sock_tcp_s.cc)
add_executable(sock_udp_c sock_udp_c.cc)
add_executable(sock_udp_s sock_udp_s.cc)
target_include_directories(sock_tcp_c PRIVATE ${SOCKET_TEST_HRD})
target_include_directories(sock_tcp_s PRIVATE ${SOCKET_TEST_HRD})
target_include_directories(sock_udp_c PRIVATE ${SOCKET_TEST_HRD})
target_include_directories(sock_udp_s PRIVATE ${SOCKET_TEST_HRD})
target_link_libraries(sock_tcp_c libgmp.so libgsocket.so libgtime.so -lrt libgret.so libgfile.so libgutil.so)
target_link_libraries(sock_tcp_s libgmp.so libgsocket.so libgtime.so -lrt libgret.so libgfile.so libgutil.so)
target_link_libraries(sock_udp_c libgmp.so libgsocket.so libgtime.so -lrt libgret.so libgfile.so libgutil.so)
target_link_libraries(sock_udp_s libgmp.so libgsocket.so libgtime.so -lrt libgret.so libgfile.so libgutil.so)
