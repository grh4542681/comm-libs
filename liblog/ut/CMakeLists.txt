set(UT_TARGETS)
set(UT_HRD "${PROJ_ROOT_PATH}"
           "${LIBBASE_SRC_TOP}"
           "${3PP_INSTALL_DIR}/include"
           )

set(UT_LOG_SRC ut_main.cc ut_log_interface.cc)
add_executable(ut_liblog ${UT_LOG_SRC})
target_include_directories(ut_liblog PRIVATE ${UT_HRD})
target_link_directories(ut_liblog PRIVATE ${3PP_INSTALL_DIR}/lib64 ${3PP_INSTALL_DIR}/lib)
target_link_libraries(ut_liblog gtest gmock pthread)
list(APPEND UT_TARGETS ut_liblog)

INSTALL(TARGETS ${UT_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/ut"
       )
