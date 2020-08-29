set(EXECUTABLE_OUTPUT_PATH ${FRAM_BIN_PATH}/test)

set(SHM_TEST_HRD "${PLAT_INFRA_PATH}/ipc"
                 "${PLAT_INFRA_PATH}/return"
                 "${PLAT_INFRA_PATH}/base"
                 "${PLAT_INFRA_PATH}/container"
                 "${PLAT_INFRA_PATH}/mempool"
                 "${PLAT_INFRA_PATH}/report"
                 "${PLAT_INFRA_PATH}/timer"
                 )

set(SYSV_SHM_TEST_SRC sysv_shm_test.cc)
add_executable(sysv_shm_test ${SYSV_SHM_TEST_SRC})
target_include_directories(sysv_shm_test PRIVATE ${SHM_TEST_HRD})

set(POSIX_SHM_TEST_SRC posix_shm_test.cc)
add_executable(posix_shm_test ${POSIX_SHM_TEST_SRC})
target_include_directories(posix_shm_test PRIVATE ${SHM_TEST_HRD})

set(SHM_LIST_TEST_SRC shm_list_test.cc)
add_executable(shm_list_test ${SHM_LIST_TEST_SRC})
target_include_directories(shm_list_test PRIVATE ${SHM_TEST_HRD})

target_link_libraries(sysv_shm_test ${COMM_OS_LIB} ${PLAT_INFRA_LIB})
target_link_libraries(posix_shm_test ${COMM_OS_LIB} ${PLAT_INFRA_LIB})
target_link_libraries(shm_list_test ${COMM_OS_LIB} ${PLAT_INFRA_LIB})
