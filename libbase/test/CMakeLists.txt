set(TEST_TARGETS)
set(TEST_HRD "${PROJ_ROOT_PATH}"
             "${LIBBASE_SRC_TOP}"
             )

set(TEST_ALLOCATOR_SRC test_allocator.cc)
add_executable(test_allocator ${TEST_ALLOCATOR_SRC})
target_include_directories(test_allocator PRIVATE ${TEST_HRD})
list(APPEND TEST_TARGETS test_allocator)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})

INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
