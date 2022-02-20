cmake_minimum_required(VERSION 3.6)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/tests/)
# Source files
set(
  SRCS
  src/tests_ThreadPool.cpp
  src/tests_traits.cpp
)

# Binary
add_executable(tests-redoom-utils ${SRCS})
target_include_directories(tests-redoom-utils PRIVATE utils)
target_link_libraries(
  tests-redoom-utils
  redoom-utils
  Catch2
  Threads::Threads
)
set_property(TARGET tests-redoom-utils PROPERTY CXX_STANDARD 20)
set_property(TARGET tests-redoom-utils PROPERTY CXX_STANDARD_REQUIRED ON)
