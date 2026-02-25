cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0048 NEW) # enable project VERSION
cmake_policy(SET CMP0056 NEW) # honor link flags in try_compile()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(zug VERSION 0.1.0)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_EXTENSIONS off)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments")
endif()

include(GNUInstallDirs)

option(DISABLE_WERROR "enable --werror")

option(zug_BUILD_TESTS "Build tests" "${PROJECT_IS_TOP_LEVEL}")
option(zug_BUILD_EXAMPLES "Build examples" "${PROJECT_IS_TOP_LEVEL}")
option(zug_BUILD_DOCS "Build docs" "${PROJECT_IS_TOP_LEVEL}")

if(NOT MSVC AND NOT DISABLE_WERROR)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

# Targets
# =======

# ccache support
find_program(CCACHE ccache)
if(CCACHE)
  message(STATUS "Using ccache: ${CCACHE}")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
  set(CMAKE_CXX_LINKER_LAUNCHER ${CCACHE})
else()
  message(STATUS "Could not find ccache")
endif()

# the library
add_library(zug INTERFACE)
add_library(zug::zug ALIAS zug)
target_include_directories(
  zug
  INTERFACE $<BUILD_INTERFACE:${zug_BINARY_DIR}/>
            $<BUILD_INTERFACE:${zug_SOURCE_DIR}/>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_sources(zug PUBLIC FILE_SET HEADERS
  FILES
    zug/any_state.hpp
    zug/compat/apply.hpp
    zug/compat/invoke.hpp
    zug/compose.hpp
    zug/detail/copy_traits.hpp
    zug/detail/empty_transducer_error.hpp
    zug/detail/inline_constexpr.hpp
    zug/detail/is_non_empty.hpp
    zug/detail/iterator_facade.hpp
    zug/detail/iterator_range.hpp
    zug/detail/lambda_wrapper.hpp
    zug/detail/reduce_nested_non_empty.hpp
    zug/detail/reduce_nested_non_empty_accumulate.hpp
    zug/detail/reduce_nested_non_empty_non_variadic.hpp
    zug/detail/reduce_nested_non_empty_nullary.hpp
    zug/detail/reduce_nested_non_empty_tail_recursive.hpp
    zug/detail/reduce_nested_non_empty_variadic.hpp
    zug/detail/tuple_utils.hpp
    zug/detail/unreachable.hpp
    zug/into.hpp
    zug/into_vector.hpp
    zug/maybe_reduced.hpp
    zug/meta.hpp
    zug/meta/common_type.hpp
    zug/meta/detected.hpp
    zug/meta/dispatch.hpp
    zug/meta/pack.hpp
    zug/meta/util.hpp
    zug/meta/value_type.hpp
    zug/reduce.hpp
    zug/reduce_nested.hpp
    zug/reducing/emplacing_back.hpp
    zug/reducing/first.hpp
    zug/reducing/last.hpp
    zug/reducing/output.hpp
    zug/reductor.hpp
    zug/run.hpp
    zug/sequence.hpp
    zug/skip.hpp
    zug/state_traits.hpp
    zug/state_wrapper.hpp
    zug/transduce.hpp
    zug/transducer/cat.hpp
    zug/transducer/chain.hpp
    zug/transducer/count.hpp
    zug/transducer/cycle.hpp
    zug/transducer/dedupe.hpp
    zug/transducer/distinct.hpp
    zug/transducer/drop.hpp
    zug/transducer/drop_while.hpp
    zug/transducer/each.hpp
    zug/transducer/eager.hpp
    zug/transducer/enumerate.hpp
    zug/transducer/filter.hpp
    zug/transducer/interleave.hpp
    zug/transducer/interpose.hpp
    zug/transducer/iter.hpp
    zug/transducer/map.hpp
    zug/transducer/map_indexed.hpp
    zug/transducer/mapcat.hpp
    zug/transducer/partition.hpp
    zug/transducer/partition_by.hpp
    zug/transducer/product.hpp
    zug/transducer/random_sample.hpp
    zug/transducer/range.hpp
    zug/transducer/read.hpp
    zug/transducer/readbuf.hpp
    zug/transducer/remove.hpp
    zug/transducer/repeat.hpp
    zug/transducer/replace.hpp
    zug/transducer/scan.hpp
    zug/transducer/sink.hpp
    zug/transducer/take.hpp
    zug/transducer/take_nth.hpp
    zug/transducer/take_while.hpp
    zug/transducer/transducer.hpp
    zug/transducer/unzip.hpp
    zug/transducer/write.hpp
    zug/transducer/writebuf.hpp
    zug/transducer/zip.hpp
    zug/tuplify.hpp
    zug/util.hpp
    zug/with_state.hpp
)

install(TARGETS zug EXPORT ZugConfig FILE_SET HEADERS)
install(EXPORT ZugConfig DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Zug")
install(DIRECTORY zug DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

if(zug_BUILD_TESTS OR zug_BUILD_EXAMPLES)
  add_custom_target(
    check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Build and run all the tests and examples.")
endif()

# the library, local development target
if(zug_BUILD_TESTS)
  enable_testing()
  find_package(Catch2 REQUIRED)

  add_library(zug-dev INTERFACE)
  target_link_libraries(zug-dev INTERFACE zug)
  if(ENABLE_COVERAGE)
    target_compile_options(zug-dev INTERFACE "--coverage")
    target_link_libraries(zug-dev INTERFACE "--coverage")
  endif()

  add_subdirectory(test)
endif()

if(zug_BUILD_EXAMPLES)
  add_subdirectory(example)
endif()

if(zug_BUILD_DOCS)
  add_subdirectory(doc)
endif()
