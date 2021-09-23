cmake_minimum_required(VERSION 3.6)

# Source files
set(
  SRCS
  src/main.cpp
  src/tests_traits.cpp
)

# Binary
add_executable(redoom-utils-tests ${SRCS})
target_include_directories(redoom-utils-tests PRIVATE utils)
target_link_libraries(redoom-utils-tests redoom-utils Catch2)
set_property(TARGET redoom-utils-tests PROPERTY CXX_STANDARD 20)
set_property(TARGET redoom-utils-tests PROPERTY CXX_STANDARD_REQUIRED ON)
