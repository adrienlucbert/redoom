cmake_minimum_required(VERSION 3.6)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/benchmarks/)
# Sources files
set(
  SRCS
  src/main.cpp
  src/bm_Allocator.cpp
)

# Binary
add_executable(benchmarks-redoom ${SRCS})
target_link_libraries(benchmarks-redoom redoom-core benchmark)
set_property(TARGET benchmarks-redoom PROPERTY CXX_STANDARD 20)
set_property(TARGET benchmarks-redoom PROPERTY CXX_STANDARD_REQUIRED ON)
