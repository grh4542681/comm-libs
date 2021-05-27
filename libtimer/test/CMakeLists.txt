set(TEST_TARGETS)
set(TEST_HRD "${PROJ_ROOT_PATH}"
             "${LIBBASE_SRC_TOP}"
             "${LIBLOG_SRC_TOP}"
             "${LIBIO_SRC_TOP}"
             "${LIBCONTAINER_SRC_TOP}"
             "${LIBTHREAD_SRC_TOP}"
             )

set(TEST_TIME_SRC test_time.cc)
add_executable(test_time ${TEST_TIME_SRC})
target_include_directories(test_time PRIVATE ${TEST_HRD})
list(APPEND TEST_TARGETS test_time)

set(TEST_DATE_SRC test_date.cc)
add_executable(test_date ${TEST_DATE_SRC})
target_include_directories(test_date PRIVATE ${TEST_HRD})
list(APPEND TEST_TARGETS test_date)

set(TEST_TIMER_SCHEDUE_SRC test_timer_schedue.cc)
add_executable(test_timer_schedue ${TEST_TIMER_SCHEDUE_SRC})
target_include_directories(test_timer_schedue PRIVATE ${TEST_HRD})
target_link_directories(test_timer_schedue PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(test_timer_schedue timer)
list(APPEND TEST_TARGETS test_timer_schedue)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})
INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
