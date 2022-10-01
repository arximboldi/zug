cmake_minimum_required(VERSION 3.8)
cmake_policy(SET CMP0048 NEW) # enable project VERSION
cmake_policy(SET CMP0056 NEW) # honor link flags in try_compile()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(zug VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_EXTENSIONS off)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments")
endif()

option(zug_BUILD_TESTS "Build tests" ON)
option(zug_BUILD_EXAMPLES "Build examples" ON)
option(zug_BUILD_DOCS "Build docs" ON)

#  Targets
#  =======

# ccache support
find_program(CCACHE ccache)
if (CCACHE)
  message(STATUS "Using ccache: ${CCACHE}")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE})
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE})
else()
  message(STATUS "Could not find ccache")
endif()

# the library
add_library(zug INTERFACE)
target_include_directories(zug INTERFACE
  $<BUILD_INTERFACE:${zug_BINARY_DIR}/>
  $<BUILD_INTERFACE:${zug_SOURCE_DIR}/>
  $<INSTALL_INTERFACE:include>)

install(TARGETS zug EXPORT ZugConfig)
install(EXPORT ZugConfig DESTINATION lib/cmake/Zug)
install(DIRECTORY zug DESTINATION include)

enable_testing()
add_custom_target(check
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Build and run all the tests and examples.")

# the library, local development target
if(zug_BUILD_TESTS)
  add_library(zug-dev INTERFACE)
  target_include_directories(zug-dev SYSTEM INTERFACE
    "$<BUILD_INTERFACE:${zug_SOURCE_DIR}/>/tools/include")
  target_link_libraries(zug-dev INTERFACE zug)
  if (ENABLE_COVERAGE)
    target_compile_options(zug-dev INTERFACE "--coverage")
    target_link_libraries(zug-dev INTERFACE "--coverage")
  endif()

  add_subdirectory(test)
endif()

if(zug_BUILD_EXAMPLES)
  add_subdirectory(example)
endif()

if (zug_BUILD_DOCS)
  add_subdirectory(doc)
endif()
