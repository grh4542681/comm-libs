set(EXECUTABLE_OUTPUT_PATH ${FRAM_BIN_PATH}/test)

set(PROCESS_TEST_HRD "${PLAT_INFRA_PATH}/process"
                     "${PLAT_INFRA_PATH}/base"
                     "${PLAT_INFRA_PATH}/file"
                     "${PLAT_INFRA_PATH}/return"
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

set(PROCESS_TEST_SRC process_test.cc)
add_executable(process_test ${PROCESS_TEST_SRC})
target_include_directories(process_test PRIVATE ${PROCESS_TEST_HRD})

set(PROCESS_SINGLE_TEST_SRC process_single_test.cc)
add_executable(process_single_test ${PROCESS_SINGLE_TEST_SRC})
target_include_directories(process_single_test PRIVATE ${PROCESS_TEST_HRD})

target_link_libraries(process_test libgprocess.so libgfile.so libgmp.so libgsocket.so libgutil.so libgthread.so libgret.so libgtime.so libgmsg.so libgio.so libgipc.so libgconf.so libgcontainer.so libgparser.so libyaml-cpp.so)
target_link_libraries(process_single_test libgprocess.so libgfile.so libgmp.so libgsocket.so libgutil.so libgthread.so libgret.so libgtime.so libgmsg.so libgio.so libgipc.so libgconf.so libgcontainer.so libgparser.so libyaml-cpp.so)
