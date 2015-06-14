add_test(NAME unit_tests COMMAND $<TARGET_FILE:database>)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(operators_unit_tests
    DummyOperator.cpp
    ../run_all_tests.cpp
    print_operator_test.cpp
    selection_operator_tests.cpp
    projection_operator_tests.cpp)
set_target_properties(operators_unit_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)
target_link_libraries(operators_unit_tests database)
target_link_libraries(operators_unit_tests gtest_main)

