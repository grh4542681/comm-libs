set(TEST_TARGETS)
set(TEST_HRD ${PROJ_ROOT_PATH}
             ${LIBPROCESS_HRD}
             )

set(TEST_PROCESS_ID_SRC test_process_id.cc)
add_executable(test_process_id ${TEST_PROCESS_ID_SRC})
target_include_directories(test_process_id PRIVATE ${TEST_HRD} ${DEPEND_INCLUDE_DIR})
target_link_directories(test_process_id PRIVATE ${CMAKE_BINARY_DIR} ${DEPEND_LIB_DIR})
target_link_libraries(test_process_id process thread file util signal pthread)
list(APPEND TEST_TARGETS test_process_id)

set(TEST_PROCESS_TEMPLATE_SRC test_process_template.cc)
add_executable(test_process_template ${TEST_PROCESS_TEMPLATE_SRC})
target_include_directories(test_process_template PRIVATE ${TEST_HRD} ${DEPEND_INCLUDE_DIR})
target_link_directories(test_process_template PRIVATE ${CMAKE_BINARY_DIR} ${DEPEND_LIB_DIR})
target_link_libraries(test_process_template process thread file util signal pthread)
list(APPEND TEST_TARGETS test_process_template)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})
INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
