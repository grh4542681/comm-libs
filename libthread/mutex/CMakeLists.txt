SET(LIBGRHTHREAD_SRC thread_mutex_lock.cc
                     thread_rw_lock.cc )
add_library(GRHthread_SO SHARED ${LIBGRHTHREAD_SRC})
add_library(GRHthread_A STATIC ${LIBGRHTHREAD_SRC})
target_link_libraries(GRHthread_SO pthread rt)
target_link_libraries(GRHthread_A pthread rt)
set_target_properties(GRHthread_SO PROPERTIES OUTPUT_NAME "gthread" CLEAN_DIRECT_OUTPUT 1)
set_target_properties(GRHthread_A PROPERTIES OUTPUT_NAME "gthread" CLEAN_DIRECT_OUTPUT 1)
