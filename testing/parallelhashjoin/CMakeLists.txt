add_test(NAME unit_tests COMMAND $<TARGET_FILE:database>)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(PARALLEL_HASH_JOIN_TEST_SOURCES
../run_all_tests.cpp
test_hash_join.cpp.cpp)
add_executable(parallel_hash_join_tests ${PARALLEL_HASH_JOIN_TEST_SOURCES})
set_target_properties(parallel_hash_join_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)
target_link_libraries(parallel_hash_join_tests gtest_main)