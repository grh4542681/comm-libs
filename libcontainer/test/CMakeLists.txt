set(TEST_TARGETS)
set(TEST_HRD "${PROJ_ROOT_PATH}"
             "${LIBBASE_SRC_TOP}"
             )

set(TEST_BITMAP_SRC test_bitmap.cc)
add_executable(test_bitmap ${TEST_BITMAP_SRC})
target_include_directories(test_bitmap PRIVATE ${TEST_HRD})
list(APPEND TEST_TARGETS test_bitmap)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})

INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
