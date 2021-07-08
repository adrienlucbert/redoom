cmake_minimum_required(VERSION 3.6)

# Source files
set(SRCS
  src/UniqueTypeId.cpp
)

# Library
add_library(redoom-utils STATIC ${SRCS})
target_include_directories(redoom-utils PUBLIC include)
# target_link_libraries(redoom-utils )
target_compile_options(redoom-utils PRIVATE ${WARNING_FLAGS})
set_property(TARGET redoom-utils PROPERTY CXX_STANDARD 20)
set_property(TARGET redoom-utils PROPERTY CXX_STANDARD_REQUIRED ON)
