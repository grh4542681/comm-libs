set(TEST_TARGETS)
set(TEST_HRD "${PROJ_ROOT_PATH}"
             "${LIBBASE_SRC_TOP}"
             )

set(TEST_FD_SRC test_fd.cc)
add_executable(test_fd ${TEST_FD_SRC})
target_include_directories(test_fd PRIVATE ${TEST_HRD})
list(APPEND TEST_TARGETS test_fd)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})
INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
