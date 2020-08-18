set(UT_TARGETS)
set(UT_HRD "${PROJ_ROOT_PATH}"
           "${LIBBASE_SRC_TOP}"
           "${3PP_INSTALL_DIR}/include"
           )

set(UT_TIMER_SRC ut_main.cc ut_timer_log.cc)
add_executable(ut_libtimer ${UT_TIMER_SRC})
target_include_directories(ut_libtimer PRIVATE ${UT_HRD})
target_link_directories(ut_libtimer PRIVATE ${3PP_INSTALL_DIR}/lib64 ${3PP_INSTALL_DIR}/lib)
target_link_libraries(ut_libtimer gtest gmock pthread)
list(APPEND UT_TARGETS ut_libtimer)

INSTALL(TARGETS ${UT_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/ut"
       )
