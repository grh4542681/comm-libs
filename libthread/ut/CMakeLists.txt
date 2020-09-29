set(UT_TARGETS)
set(UT_HRD "${PROJ_ROOT_PATH}"
           "${LIBBASE_SRC_TOP}"
           "${LIBLOG_SRC_TOP}"
           "${3PP_INSTALL_DIR}/include"
           )

set(UT_THREAD_SRC ut_main.cc ut_thread_log.cc)
add_executable(ut_libthread ${UT_THREAD_SRC})
target_include_directories(ut_libthread PRIVATE ${UT_HRD})
target_link_directories(ut_libthread PRIVATE ${3PP_INSTALL_DIR}/lib64 ${3PP_INSTALL_DIR}/lib)
target_link_libraries(ut_libthread gtest gmock pthread)
list(APPEND UT_TARGETS ut_libthread)

INSTALL(TARGETS ${UT_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/ut"
       )
