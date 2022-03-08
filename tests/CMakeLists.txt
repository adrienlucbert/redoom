cmake_minimum_required(VERSION 3.6)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/tests/)
# Source files
set(
  SRCS
  src/tests_AABB.cpp
  src/tests_Allocator.cpp
  src/tests_Body.cpp
  src/tests_Component.cpp
  src/tests_ComponentManager.cpp
  src/tests_EntityManager.cpp
  src/tests_OctTree.cpp
  src/tests_Registry.cpp
  src/tests_SystemManager.cpp
  src/tests_World.cpp
  utils/mock/systems.cpp
  utils/mock/Window.cpp
)

# Binary
add_executable(tests-redoom ${SRCS})
target_include_directories(tests-redoom PRIVATE utils)
target_link_libraries(tests-redoom redoom-core Catch2)
set_property(TARGET tests-redoom PROPERTY CXX_STANDARD 20)
set_property(TARGET tests-redoom PROPERTY CXX_STANDARD_REQUIRED ON)
