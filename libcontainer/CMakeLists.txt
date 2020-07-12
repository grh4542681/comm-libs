set(LIBGCONTAINER_HRD "${PLAT_INFRA_PATH}/return"
                      "${PLAT_INFRA_PATH}/mempool"
                      "${PLAT_INFRA_PATH}/report"
                      "${PLAT_INFRA_PATH}/util"
                      )
set(LIBGCONTAINER_SRC bitmap.cc
                      node_tree.cc
)

add_library(GRHcontainer_SO SHARED ${LIBGCONTAINER_SRC})
set_target_properties(GRHcontainer_SO PROPERTIES OUTPUT_NAME "gcontainer" CLEAN_DIRECT_OUTPUT 1)
target_include_directories(GRHcontainer_SO PRIVATE ${LIBGCONTAINER_HRD})

if(BUILD_STATIC_LIBS)
    add_library(GRHcontainer_A STATIC ${LIBGCONTAINER_SRC})
    set_target_properties(GRHcontainer_A PROPERTIES OUTPUT_NAME "gcontainer" CLEAN_DIRECT_OUTPUT 1)
    target_include_directories(GRHcontainer_A PRIVATE ${LIBGCONTAINER_HRD})
endif()
