add_test(NAME unit_tests COMMAND $<TARGET_FILE:database>)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(SLOTTED_PAGES_TEST_SOURCES main.cpp size_tests.cpp methods_tests.cpp)
add_executable(slotted_pages_tests ${SLOTTED_PAGES_TEST_SOURCES} ${SLOTTED_PAGES_SOURCES} ${BUFFER_TEST_SOURCES})
set_target_properties(slotted_pages_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)
target_link_libraries(slotted_pages_tests gtest_main)