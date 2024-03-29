cmake_minimum_required(VERSION 3.8)
cmake_policy(SET CMP0048 NEW) # enable project VERSION
cmake_policy(SET CMP0056 NEW) # honor link flags in try_compile()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(zug VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_EXTENSIONS off)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments")
endif()

include(GNUInstallDirs)

option(DISABLE_WERROR "enable --werror")

option(zug_BUILD_TESTS "Build tests" ON)
option(zug_BUILD_EXAMPLES "Build examples" ON)
option(zug_BUILD_DOCS "Build docs" ON)

if (NOT MSVC AND NOT DISABLE_WERROR)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

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
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS zug EXPORT ZugConfig)
install(EXPORT ZugConfig DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Zug")
install(DIRECTORY zug DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

enable_testing()
add_custom_target(check
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Build and run all the tests and examples.")

# the library, local development target
if(zug_BUILD_TESTS)
  find_package(Catch2 REQUIRED)

  add_library(zug-dev INTERFACE)
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
