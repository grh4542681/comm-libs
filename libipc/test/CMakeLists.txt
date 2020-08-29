set(EXECUTABLE_OUTPUT_PATH ${FRAM_BIN_PATH}/test)

set(MSG_TEST_HRD "${PLAT_INFRA_PATH}/ipc"
                 "${PLAT_INFRA_PATH}/return"
                 "${PLAT_INFRA_PATH}/timer"
                 )

set(MSG_SHM_TEST_SRC msg_shm_test.cc)
add_executable(msg_shm_test ${MSG_SHM_TEST_SRC})
target_include_directories(msg_shm_test PRIVATE ${MSG_TEST_HRD})

set(MSG_SHM_TEST2_SRC msg_shm_test2.cc)
add_executable(msg_shm_test2 ${MSG_SHM_TEST2_SRC})
target_include_directories(msg_shm_test2 PRIVATE ${MSG_TEST_HRD})

target_link_libraries(msg_shm_test ${COMM_OS_LIB} ${PLAT_INFRA_LIB})
target_link_libraries(msg_shm_test2 ${COMM_OS_LIB} ${PLAT_INFRA_LIB})
