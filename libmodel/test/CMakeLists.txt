set(TEST_TARGETS)
set(TEST_HRD "${PROJ_ROOT_PATH}"
             "${LIBBASE_SRC_TOP}"
             "${LIBLOG_SRC_TOP}"
             )

set(TEST_MODEL_SRC test_efsm.cc)
add_executable(test_efsm ${TEST_MODEL_SRC})
target_include_directories(test_efsm PRIVATE ${TEST_HRD})
list(APPEND TEST_TARGETS test_efsm)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})

INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
