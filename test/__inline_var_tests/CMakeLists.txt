
#  Targets
#  =======

message("adding unit test: inline_var_tests")

file(GLOB_RECURSE inline_var_test_files "*.cpp")
add_executable(inline_var_tests EXCLUDE_FROM_ALL ${inline_var_test_files})
add_dependencies(tests inline_var_tests)
target_link_libraries(inline_var_tests PUBLIC zug-dev)
add_test("test/inline_var_tests" inline_var_tests)
