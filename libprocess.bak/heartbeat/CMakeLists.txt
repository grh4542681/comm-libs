SET(LIBGRHHEARTBEAT_SRC heartbeat_item.cc)
add_library(GRHheartbeat_SO SHARED ${LIBGRHHEARTBEAT_SRC})
add_library(GRHheartbeat_A STATIC ${LIBGRHHEARTBEAT_SRC})
set_target_properties(GRHheartbeat_SO PROPERTIES OUTPUT_NAME "gheartbeat" CLEAN_DIRECT_OUTPUT 1)
set_target_properties(GRHheartbeat_A PROPERTIES OUTPUT_NAME "gheartbeat" CLEAN_DIRECT_OUTPUT 1)

