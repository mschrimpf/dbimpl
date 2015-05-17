add_test(NAME unit_tests COMMAND $<TARGET_FILE:database>)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(RELATIONS_TEST_SOURCES test_schema_serializer.cpp main.cpp)
add_executable(relations_tests ${RELATIONS_TEST_SOURCES})
set_target_properties(relations_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)
target_link_libraries(relations_tests database)
target_link_libraries(relations_tests gtest_main)