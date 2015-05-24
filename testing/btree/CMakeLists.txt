add_test(NAME unit_tests COMMAND $<TARGET_FILE:database>)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(BTREE_TEST_SOURCES
../run_all_tests.cpp
BTreeTest.cpp)
add_executable(btree_tests ${BTREE_TEST_SOURCES} ../../src/database/btree/BTree.cpp ../../src/database/btree/BTree.h test_btree_methods.h test_btree_methods.cpp)
set_target_properties(btree_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)
target_link_libraries(btree_tests database)
target_link_libraries(btree_tests gtest_main)