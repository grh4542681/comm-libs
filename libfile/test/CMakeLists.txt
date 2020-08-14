set(TEST_TARGETS)
set(TEST_HRD "${PROJ_ROOT_PATH}"
             "${LIBBASE_SRC_TOP}"
             "${LIBIO_SRC_TOP}"
             )

set(TEST_FILE_SRC test_file.cc)
add_executable(test_file ${TEST_FILE_SRC})
target_include_directories(test_file PRIVATE ${TEST_HRD})
target_link_directories(test_file PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(test_file libfile.so libutil.so)
list(APPEND TEST_TARGETS test_file)

set(TEST_PATH_SRC test_path.cc)
add_executable(test_path ${TEST_PATH_SRC})
target_include_directories(test_path PRIVATE ${TEST_HRD})
target_link_directories(test_path PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(test_path libfile.so libutil.so)
list(APPEND TEST_TARGETS test_path)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})
INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
