add_test(NAME unit_tests COMMAND $<TARGET_FILE:database>)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(arithmetic_expression_test arithmetic_expression_test.cpp
    ../run_all_tests.cpp
    ../../src/assignment6_codegeneration/arithmetic_expressions.h
    ../../src/assignment6_codegeneration/arithmetic_expressions.cpp
    ../../src/assignment6_codegeneration/Node.h
    ../../src/assignment6_codegeneration/Node.cpp
    ../../src/assignment6_codegeneration/FunctionContext.h
    ../../src/assignment6_codegeneration/FunctionContext.cpp)
set_target_properties(arithmetic_expression_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)
target_link_libraries(arithmetic_expression_test gtest_main)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})


# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs core executionengine interpreter mc mcjit support nativecodegen)

# Link against LLVM libraries
target_link_libraries(arithmetic_expression_test ${llvm_libs})

