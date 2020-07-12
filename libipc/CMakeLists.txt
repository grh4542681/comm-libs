set(LIBGRHIPC_HRD "${PLAT_INFRA_PATH}/return"
                  "${PLAT_INFRA_PATH}/base"
                  "${PLAT_INFRA_PATH}/ipc"
                  "${PLAT_INFRA_PATH}/mempool"
                  "${PLAT_INFRA_PATH}/report"
                  "${PLAT_INFRA_PATH}/timer"
                  "${PLAT_INFRA_PATH}/socket"
                  "${PLAT_INFRA_PATH}/io"
                  )
set(LIBGRHIPC_SRC ipc_mode.cc
                  ipc_return.cc
                  shm/shm_sysv.cc
                  shm/shm_posix.cc
                  sem/sem_sysv.cc
                  sem/sem_posix.cc
                  msg/msg_shm.cc
                  socket/sock_pair.cc
                  event/event_fd.cc
                  event/event_semaphore.cc
                  )

add_library(GRHipc_SO SHARED ${LIBGRHIPC_SRC})
target_include_directories(GRHipc_SO PRIVATE ${LIBGRHIPC_HRD})
set_target_properties(GRHipc_SO PROPERTIES OUTPUT_NAME "gipc" CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(GRHipc_SO rt)

if(BUILD_STATIC_LIBS)
    add_library(GRHipc_A SHARED ${LIBGRHIPC_SRC})
    target_include_directories(GRHipc_A PRIVATE ${LIBGRHIPC_HRD})
    set_target_properties(GRHipc_A PROPERTIES OUTPUT_NAME "gipc" CLEAN_DIRECT_OUTPUT 1)
    target_link_libraries(GRHipc_A rt)
endif()
