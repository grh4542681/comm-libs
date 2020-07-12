set(LIBGRHTHREAD_HRD "${PLAT_INFRA_PATH}/return"
                     "${PLAT_INFRA_PATH}/process"
                     "${PLAT_INFRA_PATH}/file"
                     "${PLAT_INFRA_PATH}/io"
                     "${PLAT_INFRA_PATH}/mempool"
                     "${PLAT_INFRA_PATH}/report"
                     "${PLAT_INFRA_PATH}/thread"
                     "${PLAT_INFRA_PATH}/ipc"
                     "${PLAT_INFRA_PATH}/socket"
                     "${PLAT_INFRA_PATH}/timer"
                     "${PLAT_INFRA_PATH}/container"
                     "${PLAT_INFRA_PATH}/util"
                     "${PLAT_INFRA_PATH}/parser"
                     "${PLAT_INFRA_PATH}/base"
                     "${FRAM_PLAT_PATH}/config/lib"
                     "${3PP_RAPIDJSON_HRD}"
                     "${3PP_YAML_HRD}"
                     "${3PP_PROTOBUF_HRD}"
                     )
set(LIBGRHTHREAD_SRC thread_return.cc
                     thread_id.cc
                     thread_info.cc
                     mutex/thread_mutex_lock.cc
                     mutex/thread_rw_lock.cc 
                     signal/thread_signal_ctrl.cc
                     )

add_library(GRHthread_SO SHARED ${LIBGRHTHREAD_SRC})
target_include_directories(GRHthread_SO PRIVATE ${LIBGRHTHREAD_HRD})
target_link_libraries(GRHthread_SO rt)
set_target_properties(GRHthread_SO PROPERTIES OUTPUT_NAME "gthread" CLEAN_DIRECT_OUTPUT 1)

if(BUILD_STATIC_LIBS)
    add_library(GRHthread_A STATIC ${LIBGRHTHREAD_SRC})
    target_include_directories(GRHthread_A PRIVATE ${LIBGRHTHREAD_HRD})
    target_link_libraries(GRHthread_A rt)
    set_target_properties(GRHthread_A PROPERTIES OUTPUT_NAME "gthread" CLEAN_DIRECT_OUTPUT 1)
endif()
