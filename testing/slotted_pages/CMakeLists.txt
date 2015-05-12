add_test(NAME unit_tests COMMAND $<TARGET_FILE:database>)

set(SLOTTED_PAGES_TEST_SOURCES unit_tests.cpp)
add_executable(slotted_pages_tests ${SLOTTED_PAGES_TEST_SOURCES} ${BUFFER_TEST_SOURCES})
set_target_properties(slotted_pages_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)
target_link_libraries(slotted_pages_tests gtest_main)