set(UT_TARGETS)
set(UT_HRD "${PROJ_ROOT_PATH}"
           "${LIBBASE_SRC_TOP}"
           "${3PP_INSTALL_DIR}/include"
           )

set(UT_UTIL_SRC ut_main.cc ut_string_util.cc)
add_executable(ut_libutil ${UT_UTIL_SRC})
target_include_directories(ut_libutil PRIVATE ${UT_HRD})
target_link_directories(ut_libutil PRIVATE ${3PP_INSTALL_DIR}/lib64 ${3PP_INSTALL_DIR}/lib)
target_link_libraries(ut_libutil gtest gmock pthread)
list(APPEND UT_TARGETS ut_libutil)

INSTALL(TARGETS ${UT_TARGETS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/ut"
       )
