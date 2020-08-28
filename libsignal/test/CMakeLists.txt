set(TEST_TARGETS)
set(TEST_HRD ${PROJ_ROOT_PATH}
             ${LIBSIGNAL_HRD}
             )

set(TEST_SIGNAL_ID_SRC test_signal_id.cc)
add_executable(test_signal_id ${TEST_SIGNAL_ID_SRC})
target_include_directories(test_signal_id PRIVATE ${TEST_HRD} ${DEPEND_INCLUDE_DIR})
target_link_directories(test_signal_id PRIVATE ${CMAKE_BINARY_DIR} ${DEPEND_LIB_DIR})
target_link_libraries(test_signal_id signal pthread)
list(APPEND TEST_TARGETS test_signal_id)

add_custom_target(test)
add_dependencies(test ${TEST_TARGETS})
INSTALL(TARGETS ${TEST_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/test"
       )
