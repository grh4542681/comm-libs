set(LIBGRHPARSER_HRD "${PLAT_INFRA_PATH}/return"
                     "${PLAT_INFRA_PATH}/mempool"
                     "${PLAT_INFRA_PATH}/report"
                     "${PLAT_INFRA_PATH}/file"
                     "${PLAT_INFRA_PATH}/io"
                     "${PLAT_INFRA_PATH}/thread"
                     "${PLAT_INFRA_PATH}/base"
                     "${PLAT_INFRA_PATH}/util"
                     "${3PP_RAPIDJSON_HRD}"
                     "${3PP_YAML_HRD}"
                     "${3PP_PROTOBUF_HRD}"
                     )
set(LIBGRHPARSER_SRC parser_return.cc
                     parser_ini.cc
                     parser_json.cc
                     parser_yaml.cc
                     parser_tvl.cc
                     )

add_library(GRHparser_SO SHARED ${LIBGRHPARSER_SRC})
target_include_directories(GRHparser_SO PRIVATE ${LIBGRHPARSER_HRD})
set_target_properties(GRHparser_SO PROPERTIES OUTPUT_NAME "gparser" CLEAN_DIRECT_OUTPUT 1)

if(BUILD_STATIC_LIBS)
    add_library(GRHparser_A STATIC ${LIBGRHPARSER_SRC})
    target_include_directories(GRHparser_SO PRIVATE ${LIBGRHPARSER_HRD})
    set_target_properties(GRHparser_A PROPERTIES OUTPUT_NAME "gparser" CLEAN_DIRECT_OUTPUT 1)
endif()
