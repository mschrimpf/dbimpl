add_test(NAME unit_tests COMMAND $<TARGET_FILE:database>)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
add_executable(btree_tests
    ../run_all_tests.cpp
    BTreeTest.cpp
    test_btree_methods.cpp)
set_target_properties(btree_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)
target_link_libraries(btree_tests database)
target_link_libraries(btree_tests gtest_main)