add_test(NAME unit_tests COMMAND $<TARGET_FILE:database>)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(btree_unit_tests
    ../run_all_tests.cpp
    size_tests.cpp
    BTreeTestFixture.hpp
    insert_tests.cpp
    test_btree_methods.cpp)
set_target_properties(btree_unit_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)
target_link_libraries(btree_unit_tests database)
target_link_libraries(btree_unit_tests gtest_main)

add_executable(BTreeTest
    BTreeTest.cpp)
set_target_properties(BTreeTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)
target_link_libraries(BTreeTest database)
target_link_libraries(BTreeTest gtest_main)