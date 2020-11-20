set(TEST_TARGETS)
set(TEST_HRD "${PROJ_ROOT_PATH}"
             "${LIBBASE_SRC_TOP}"
             "${LIBLOG_SRC_TOP}"
             )

set(TEST_BITMAP_SRC test_bitmap.cc)
add_executable(test_bitmap ${TEST_BITMAP_SRC})
target_include_directories(test_bitmap PRIVATE ${TEST_HRD})
list(APPEND TEST_TARGETS test_bitmap)

set(TEST_UNION_KEY_SRC test_union_key.cc)
add_executable(test_union_key ${TEST_UNION_KEY_SRC})
target_include_directories(test_union_key PRIVATE ${TEST_HRD})
list(APPEND TEST_TARGETS test_union_key)

set(TEST_SEARCH_TABLE_SRC test_search_table.cc)
add_executable(test_search_table ${TEST_SEARCH_TABLE_SRC})
target_include_directories(test_search_table PRIVATE ${TEST_HRD})
list(APPEND TEST_TARGETS test_search_table)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})

INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
