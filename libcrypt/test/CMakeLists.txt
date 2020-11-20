set(TEST_TARGETS)
set(TEST_HRD "${PROJ_ROOT_PATH}"
             "${LIBBASE_SRC_TOP}"
             "${LIBLOG_SRC_TOP}"
             )

set(TEST_SNOWFLAKE_SRC test_snowflake.cc)
add_executable(test_snowflake ${TEST_SNOWFLAKE_SRC})
target_include_directories(test_snowflake PRIVATE ${TEST_HRD})
list(APPEND TEST_TARGETS test_snowflake)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})

INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
