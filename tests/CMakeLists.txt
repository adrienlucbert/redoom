cmake_minimum_required(VERSION 3.6)

# Source files
set(SRCS
  src/main.cpp
  src/tests_Allocator.cpp
  src/tests_Component.cpp
  src/tests_ComponentManager.cpp
  src/tests_Entity.cpp
  src/tests_EntityManager.cpp
)

# Binary
add_executable(tests-redoom ${SRCS})
target_include_directories(tests-redoom PRIVATE .)
target_link_libraries(tests-redoom redoom-core Catch2)
set_property(TARGET tests-redoom PROPERTY CXX_STANDARD 20)
set_property(TARGET tests-redoom PROPERTY CXX_STANDARD_REQUIRED ON)
