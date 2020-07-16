set(TEST_TARGETS)
set(TEST_HRD "${PROJ_ROOT_PATH}"
             "${LIBLOGGERR_SRC_TOP}"
             )

set(TEST_LOGGER_SRC test_logger.cc)
add_executable(test_logger ${TEST_LOGGER_SRC})
target_include_directories(test_logger PRIVATE ${TEST_HRD})
list(APPEND TEST_TARGETS test_logger)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})

INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
