set(TEST_TARGETS)
set(TEST_HRD ${PROJ_ROOT_PATH}
             ${LIBPROCESS_HRD}
             )

set(TEST_PROCESS_ID_SRC test_process_id.cc)
add_executable(test_process_id ${TEST_PROCESS_ID_SRC})
target_include_directories(test_process_id PRIVATE ${TEST_HRD} ${DEPEND_INCLUDE_DIR})
target_link_directories(test_process_id PRIVATE ${CMAKE_BINARY_DIR} ${DEPEND_LIB_DIR})
target_link_libraries(test_process_id libprocess.so libfile.so libutil.so)
list(APPEND TEST_TARGETS test_process_id)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})
INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
