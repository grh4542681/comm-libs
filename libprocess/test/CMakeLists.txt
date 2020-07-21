set(TEST_TARGETS)
set(TEST_HRD "${PROJ_ROOT_PATH}"
             "${LIBBASE_SRC_TOP}"
             "${LIBLOG_SRC_TOP}"
             )

set(TEST_PROCESS_ID_SRC test_process_id.cc)
add_executable(test_process_id ${TEST_PROCESS_ID_SRC})
target_include_directories(test_process_id PRIVATE ${TEST_HRD})
target_link_libraries(test_process_id libprocess.so)
list(APPEND TEST_TARGETS test_process_id)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})
INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
