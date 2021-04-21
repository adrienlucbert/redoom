cmake_minimum_required(VERSION 3.6)

project("redoom" VERSION 0.1
                 LANGUAGES CXX)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake-modules")
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/)

option(REDOOM_BUILD_TESTS "Build the tests" ON)
option(REDOOM_BUILD_TOOLS "Build the tools" ON)

add_subdirectory(deps)

# Warning flags
set(GCC_WARNING_FLAGS
  -Wmisleading-indentation
  -Wduplicated-cond
  -Wduplicated-branches
  -Wlogical-op
  -Wnull-dereference
  -Wuseless-cast
)

set(CLANG_WARNING_FLAGS
  -Weverything
  -Wno-c++98-c++11-compat-binary-literal
  -Wno-c++98-compat
  -Wno-c++98-compat-pedantic
  -Wno-c99-extensions
  -Wno-disabled-macro-expansion
  -Wno-documentation-unknown-command
  -Wno-exit-time-destructors
  -Wno-global-constructors
  -Wno-implicit-int-float-conversion
  -Wno-return-std-move
)

if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 13.0.0)
  set(CLANG_WARNING_FLAGS ${CLANG_WARNING_FLAGS} -Wno-return-std-move-in-c++11)
endif()

set(WARNING_FLAGS
  -Wall
  -Wextra
  -Wshadow
  -Wnon-virtual-dtor
  -pedantic
  -Wold-style-cast
  -Wcast-align
  -Wunused
  -Woverloaded-virtual
  -Wpedantic
  -Wconversion
  -Wsign-conversion
  -Wdouble-promotion
  -Wformat=2
  -Weffc++
  -Wno-padded
  -Wno-ctad-maybe-unsupported
)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
  set(CXX_FLAGS ${CXX_FLAGS} -ggdb3)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(WARNING_FLAGS ${WARNING_FLAGS} ${CLANG_WARNING_FLAGS})
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(WARNING_FLAGS ${WARNING_FLAGS} ${GCC_WARNING_FLAGS})
else()
  error("${CMAKE_CXX_COMPILER_ID}: Unsupported compiler.")
endif()

set(WARNING_FLAGS ${WARNING_FLAGS} ${REDOOM_WARNING_FLAGS})
message("Warning flags: ${WARNING_FLAGS}")

# Clang-tidy
if (USE_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES "clang-tidy" DOC "Path to clang-tidy executable")
  if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    message("-- Found clang-tidy: ${CLANG_TIDY_EXE}")
  else()
    message("-- Failed to find clang-tidy")
  endif()
endif()

add_executable(redoom
  src/redoom/main.cpp
)
target_link_options(redoom PRIVATE -rdynamic)
target_compile_options(redoom PRIVATE ${WARNING_FLAGS})
target_include_directories(redoom PUBLIC src)
set_property(TARGET redoom PROPERTY CXX_STANDARD 20)
set_property(TARGET redoom PROPERTY CXX_STANDARD_REQUIRED ON)

if (REDOOM_BUILD_TESTS)
  add_subdirectory(tests)
endif()
