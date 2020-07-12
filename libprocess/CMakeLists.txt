set(LIBGPROCESS_HRD "${PLAT_INFRA_PATH}/return"
                    "${PLAT_INFRA_PATH}/base"
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
                    "${FRAM_PLAT_PATH}/config/lib"
                    "${3PP_RAPIDJSON_HRD}"
                    "${3PP_YAML_HRD}"
                    "${3PP_PROTOBUF_HRD}"
                    )
set(LIBGPROCESS_SRC process_return.cc
                    process_info.cc
                    process.cc
                    process_role.cc
                    process_id.cc
                    process_parent.cc
                    process_child.cc
                    process_config.cc
                    signal/process_signal.cc
                    signal/process_signal_ctrl.cc
                    signal/process_signal_set.cc
                    signal/process_signal_action.cc
                    group/process_group_worker.cc
                    )

add_library(GRHprocess_SO SHARED ${LIBGPROCESS_SRC})
target_include_directories(GRHprocess_SO PRIVATE ${LIBGPROCESS_HRD})
set_target_properties(GRHprocess_SO PROPERTIES OUTPUT_NAME "gprocess" CLEAN_DIRECT_OUTPUT 1)

if(BUILD_STATIC_LIBS)
    add_library(GRHprocess_A STATIC ${LIBGPROCESS_SRC})
    target_include_directories(GRHprocess_A PRIVATE ${LIBGPROCESS_HRD})
    set_target_properties(GRHprocess_A PROPERTIES OUTPUT_NAME "gprocess" CLEAN_DIRECT_OUTPUT 1)
endif()
