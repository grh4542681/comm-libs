set(LIBGRHFSM_HRD "${PLAT_INFRA_PATH}/return"
                  "${PLAT_INFRA_PATH}/fsm"
                  )

set(LIBGRHFSM_SRC fsm_return.cc
                  )

add_library(GRHfsm_SO SHARED ${LIBGRHFSM_SRC})
set_target_properties(GRHfsm_SO PROPERTIES OUTPUT_NAME "gfsm" CLEAN_DIRECT_OUTPUT 1)
target_include_directories(GRHfsm_SO PRIVATE ${LIBGRHFSM_HRD})
target_link_libraries(GRHfsm_SO rt)

if(BUILD_STATIC_LIBS)
    add_library(GRHfsm_A STATIC ${LIBGRHFSM_SRC})
    set_target_properties(GRHfsm_A PROPERTIES OUTPUT_NAME "gfsm" CLEAN_DIRECT_OUTPUT 1)
    target_include_directories(GRHfsm_A PRIVATE ${LIBGRHFSM_HRD})
endif()
