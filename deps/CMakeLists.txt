# disable cmake warnings for dependencies
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
# honor normal variables in options
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# fmt
set(BUILD_SHARED_LIBS on)
add_subdirectory(fmt)

# spdlog
add_subdirectory(spdlog)

# tl
add_subdirectory(tl)

# yaml-cpp
set(YAML_CPP_BUILD_CONTRIB off)
set(YAML_CPP_BUILD_TOOLS off)
set(YAML_BUILD_SHARED_LIBS on)
set(YAML_CPP_BUILD_TESTS off)
set(YAML_CPP_INSTALL off)
add_subdirectory(yaml-cpp)

# Catch2
if(REDOOM_BUILD_TESTS)
  set(OLD_CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD})
  set(CMAKE_CXX_STANDARD 17)
  set(CATCH_BUILD_STATIC_LIBRARY ON)
  add_subdirectory(Catch2)
  target_link_libraries(Catch2 INTERFACE Catch2::Catch2WithMain)
  set(CMAKE_CXX_STANDARD ${OLD_CMAKE_CXX_STANDARD})
  unset(OLD_CMAKE_CXX_STANDARD)
endif()

# Google Benchmark
if(REDOOM_BUILD_BENCHMARKS)
  set(BENCHMARK_ENABLE_TESTING OFF)
  set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
  set(BENCHMARK_ENABLE_INSTALL OFF)
  add_subdirectory(benchmark)
endif()

# stb
add_subdirectory(stb)

# assimp
set(ASSIMP_BUILD_ASSIMP_TOOLS off)
set(ASSIMP_BUILD_TESTS off)
if(NOT CMAKE_BUILD_TYPE MATCHES "Debug")
  set(INJECT_DEBUG_POSTFIX off)
endif()
add_subdirectory(assimp)

# glfw
add_subdirectory(glfw EXCLUDE_FROM_ALL)

# glew
add_definitions(-DGLEW_SHARED)
add_subdirectory(glew EXCLUDE_FROM_ALL)
add_library(glew ALIAS libglew_shared)

# glm
add_subdirectory(glm EXCLUDE_FROM_ALL)

# imgui
add_subdirectory(imgui)
