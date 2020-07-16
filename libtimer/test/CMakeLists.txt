set(TEST_TARGETS)
set(TEST_HRD "${PROJ_ROOT_PATH}"
             "${LIBBASE_SRC_TOP}"
             "${LIBLOG_SRC_TOP}"
             )

set(TEST_TIME_SRC test_time.cc)
add_executable(test_time ${TEST_TIME_SRC})
target_include_directories(test_time PRIVATE ${TEST_HRD})
list(APPEND TEST_TARGETS test_time)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})
INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
