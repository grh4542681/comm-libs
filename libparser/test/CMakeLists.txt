set(EXECUTABLE_OUTPUT_PATH ${FRAM_BIN_PATH}/test)

set(PARSER_TEST_HRD "${PLAT_INFRA_PATH}/return"
                    "${PLAT_INFRA_PATH}/base"
                    "${PLAT_INFRA_PATH}/mempool"
                    "${PLAT_INFRA_PATH}/report"
                    "${PLAT_INFRA_PATH}/parser"
                    "${PLAT_INFRA_PATH}/file"
                    "${PLAT_INFRA_PATH}/io"
                    "${PLAT_INFRA_PATH}/thread"
                    "${3PP_RAPIDJSON_HRD}"
                    "${3PP_YAML_HRD}"
                    "${3PP_PROTOBUF_HRD}"
                    )

set(PARSER_INI_TEST_SRC parser_ini_test.cc)
add_executable(parser_ini_test ${PARSER_INI_TEST_SRC})
target_include_directories(parser_ini_test PRIVATE ${PARSER_TEST_HRD})
target_link_libraries(parser_ini_test libgparser.so libgthread.so libgprocess.so libgsocket.so libgfile.so libgmp.so libgret.so libgtime.so libgutil.so libgmsg.so libgio.so libgcontainer.so libgconf.so libgparser.so libyaml-cpp.so)

set(PARSER_JSON_TEST_SRC parser_json_test.cc)
add_executable(parser_json_test ${PARSER_JSON_TEST_SRC})
target_include_directories(parser_json_test PRIVATE ${PARSER_TEST_HRD})
target_link_libraries(parser_json_test ${COMMON_OS_LIB} ${PLAT_INFRA_LIB} libgret.so libgmsg.so libgio.so libgcontainer.so libgconf.so libgparser.so libyaml-cpp.so)

set(PARSER_YAML_TEST_SRC parser_yaml_test.cc)
add_executable(parser_yaml_test ${PARSER_YAML_TEST_SRC})
target_include_directories(parser_yaml_test PRIVATE ${PARSER_TEST_HRD})
target_link_libraries(parser_yaml_test libgparser.so libgthread.so libgprocess.so libgsocket.so libgfile.so libgmp.so libgret.so libgtime.so libgutil.so libgmsg.so libgio.so libgcontainer.so libgconf.so libgparser.so libyaml-cpp.so)

set(PARSER_TVL_TEST_SRC parser_tvl_test.cc)
add_executable(parser_tvl_test ${PARSER_TVL_TEST_SRC})
target_include_directories(parser_tvl_test PRIVATE ${PARSER_TEST_HRD})
target_link_libraries(parser_tvl_test libgparser.so libprotobuf.so libprotoc.so)
