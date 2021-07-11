cmake_policy(SET CMP0077 NEW)

add_subdirectory(fmt)
add_subdirectory(spdlog)
add_subdirectory(tl)

# Catch2
if (REDOOM_BUILD_TESTS)
  set(OLD_CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD})
  set(CMAKE_CXX_STANDARD 17)
  add_subdirectory(Catch2)
  set(CMAKE_CXX_STANDARD ${OLD_CMAKE_CXX_STANDARD})
  unset(OLD_CMAKE_CXX_STANDARD)
endif()

# Google Benchmark
if (REDOOM_BUILD_BENCHMARKS)
  set(BENCHMARK_ENABLE_TESTING OFF)
  set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
  set(BENCHMARK_ENABLE_INSTALL OFF)
  add_subdirectory(benchmark)
endif()

# glfw
add_subdirectory(glfw EXCLUDE_FROM_ALL)

# glew
add_definitions(-DGLEW_STATIC)
add_subdirectory(glew EXCLUDE_FROM_ALL)
add_library(glew ALIAS libglew_static)

# glm
add_subdirectory(glm EXCLUDE_FROM_ALL)
