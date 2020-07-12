set(EXECUTABLE_OUTPUT_PATH ${FRAM_BIN_PATH}/test)

set(CONTAINER_TEST_HRD "${PLAT_INFRA_PATH}/return"
                       "${PLAT_INFRA_PATH}/container"
                       "${PLAT_INFRA_PATH}/mempool"
                       "${PLAT_INFRA_PATH}/report"
                       "${PLAT_INFRA_PATH}/util"
                       "${PLAT_INFRA_PATH}/thread"
                       )

set(BITMAP_TEST_SRC bitmap_test.cc)
set(TEST_LINK_LIST_SRC test_link_list.cc)
set(TEST_HASH_TABLE_SRC test_hash_table.cc)
set(TEST_TRIE_TREE_SRC test_trie_tree.cc)
set(TEST_NODE_TREE_SRC test_node_tree.cc)

add_executable(bitmap_test ${BITMAP_TEST_SRC})
add_executable(test_link_list ${TEST_LINK_LIST_SRC})
add_executable(test_hash_table ${TEST_HASH_TABLE_SRC})
add_executable(test_trie_tree ${TEST_TRIE_TREE_SRC})
add_executable(test_node_tree ${TEST_NODE_TREE_SRC})

target_include_directories(bitmap_test PRIVATE ${CONTAINER_TEST_HRD})
target_include_directories(test_link_list PRIVATE ${CONTAINER_TEST_HRD})
target_include_directories(test_hash_table PRIVATE ${CONTAINER_TEST_HRD})
target_include_directories(test_trie_tree PRIVATE ${CONTAINER_TEST_HRD})
target_include_directories(test_node_tree PRIVATE ${CONTAINER_TEST_HRD})

target_link_libraries(bitmap_test libgcontainer.so librt.so libgmp.so libgret.so libgfile.so libgutil.so)
target_link_libraries(test_link_list libgmp.so pthread libgthread.so libgprocess.so libgipc.so libgsocket.so libgfile.so libgutil.so libgret.so libgmsg.so libgio.so libgtime.so libgcontainer.so libgconf.so libgparser.so libyaml-cpp.so)
target_link_libraries(test_hash_table libgmp.so pthread libgthread.so libgprocess.so libgipc.so libgsocket.so libgfile.so libgutil.so libgret.so libgmsg.so libgio.so libgtime.so libgcontainer.so libgconf.so libgparser.so libyaml-cpp.so)
target_link_libraries(test_trie_tree libgmp.so pthread libgthread.so libgprocess.so libgipc.so libgsocket.so libgfile.so libgutil.so libgret.so libgmsg.so libgio.so libgtime.so libgcontainer.so libgconf.so libgparser.so libyaml-cpp.so)
target_link_libraries(test_node_tree libgcontainer.so libgmp.so libgret.so libgfile.so libgio.so libgtime.so libgutil.so)
