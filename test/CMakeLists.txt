#  Utils
#  =====

function(zug_target_name_for out_target out_file file)
  get_filename_component(_extension ${_file} EXT)

  file(RELATIVE_PATH _relative ${CMAKE_CURRENT_SOURCE_DIR} ${file})
  string(REPLACE "${_extension}" "" _name ${_relative})
  string(REGEX REPLACE "/" "-" _name ${_name})
  set(${out_target} "test-${_name}" PARENT_SCOPE)

  string(REPLACE "${_extension}" "" _name ${_relative})
  string(REGEX REPLACE "/" "-" _name ${_name})
  set(${out_file} "${_name}" PARENT_SCOPE)
endfunction()

#  Targets
#  =======

add_custom_target(tests COMMENT "Build all the unit tests.")
add_dependencies(check tests)

add_subdirectory(__inline_var_tests)

file(GLOB_RECURSE zug_unit_tests "*.cpp")
list(FILTER zug_unit_tests EXCLUDE REGEX "${CURRENT_CMAKE_DIRECTORY}/__*")

foreach(_file IN LISTS zug_unit_tests)
  message("found unit test: " ${_file})
  zug_target_name_for(_target _output "${_file}")
  add_executable(${_target} EXCLUDE_FROM_ALL "${_file}")
  set_target_properties(${_target} PROPERTIES OUTPUT_NAME ${_output})
  add_dependencies(tests ${_target})
  target_compile_definitions(${_target} PUBLIC CATCH_CONFIG_MAIN)
  target_link_libraries(${_target} PUBLIC zug-dev)
  add_test("test/${_output}" ${_output})
endforeach()
