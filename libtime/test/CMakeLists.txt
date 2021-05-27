set(TEST_TARGETS)
set(TEST_HRD "${PROJ_ROOT_PATH}"
             "${LIBBASE_SRC_TOP}"
             "${LIBLOG_SRC_TOP}"
             "${LIBIO_SRC_TOP}"
             "${LIBCONTAINER_SRC_TOP}"
             "${LIBTHREAD_SRC_TOP}"
             )

set(TEST_TIME_SRC test_time_point.cc)
add_executable(test_time_point ${TEST_TIME_SRC})
target_include_directories(test_time_point PRIVATE ${TEST_HRD})
target_link_directories(test_time_point PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(test_time_point time)
list(APPEND TEST_TARGETS test_time_point)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})
INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
