set(TEST_TARGETS)
set(TEST_HRD ${PROJ_ROOT_PATH}
             ${LIBTHREAD_HRD}
             )

set(TEST_THREAD_ID_SRC test_thread_id.cc)
add_executable(test_thread_id ${TEST_THREAD_ID_SRC})
target_include_directories(test_thread_id PRIVATE ${TEST_HRD} ${DEPEND_INCLUDE_DIR})
target_link_directories(test_thread_id PRIVATE ${CMAKE_BINARY_DIR} ${DEPEND_LIB_DIR})
target_link_libraries(test_thread_id thread pthread)
list(APPEND TEST_TARGETS test_thread_id)

#set(TEST_THREAD_TEMPLATE_SRC test_thread_template.cc)
#add_executable(test_thread_template ${TEST_THREAD_TEMPLATE_SRC})
#target_include_directories(test_thread_template PRIVATE ${TEST_HRD} ${DEPEND_INCLUDE_DIR})
#target_link_directories(test_thread_template PRIVATE ${CMAKE_BINARY_DIR} ${DEPEND_LIB_DIR})
#target_link_libraries(test_thread_template thread file util signal pthread)
#list(APPEND TEST_TARGETS test_thread_template)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})
INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
