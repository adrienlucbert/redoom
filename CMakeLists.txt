cmake_minimum_required(VERSION 3.6)
project(redoom)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake-modules")
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

option(REDOOM_BUILD_TESTS "Build the tests" ON)
option(REDOOM_BUILD_BENCHMARKS "Build the benchmarks" ON)
option(REDOOM_BUILD_EXAMPLES "Build the examples" ON)

add_subdirectory(deps)
add_subdirectory(modules)

# Warning flags
set(
  GCC_WARNING_FLAGS
  -Wmisleading-indentation
  -Wduplicated-cond
  -Wduplicated-branches
  -Wlogical-op
  -Wnull-dereference
  -Wuseless-cast
)

set(
  CLANG_WARNING_FLAGS
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
  -Wno-c++20-compat
  -Wno-signed-enum-bitfield
  -Wno-weak-vtables
)

if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 13.0.0)
  set(CLANG_WARNING_FLAGS ${CLANG_WARNING_FLAGS} -Wno-return-std-move-in-c++11)
endif()

set(
  WARNING_FLAGS
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

set(
  CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}
  CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel tsan"
  FORCE
)
if(CMAKE_BUILD_TYPE MATCHES "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3")
elseif(CMAKE_BUILD_TYPE MATCHES "tsan")
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=thread")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(WARNING_FLAGS ${WARNING_FLAGS} ${CLANG_WARNING_FLAGS})
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(WARNING_FLAGS ${WARNING_FLAGS} ${GCC_WARNING_FLAGS})
else()
  error("${CMAKE_CXX_COMPILER_ID}: Unsupported compiler.")
endif()

set(WARNING_FLAGS ${WARNING_FLAGS} ${REDOOM_WARNING_FLAGS})
message("Warning flags: ${WARNING_FLAGS}")

# Clang-tidy
if(USE_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES "clang-tidy" DOC "Path to clang-tidy executable")
  if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    message("-- Found clang-tidy: ${CLANG_TIDY_EXE}")
  else()
    message("-- Failed to find clang-tidy")
  endif()
endif()

add_library(
  redoom-core STATIC
  src/Application.cpp
  src/ecs/components/BehaviourComponent.cpp
  src/ecs/EntityManager.cpp
  src/ecs/Registry.cpp
  src/ecs/SystemBase.cpp
  src/ecs/SystemManager.cpp
  src/ecs/systems/BehaviourSystem.cpp
  src/ecs/systems/CameraSystem.cpp
  src/ecs/systems/MeshSystem.cpp
  src/ecs/UpdateContext.cpp
  src/graphics/BufferLayout.cpp
  src/graphics/Camera.cpp
  src/graphics/Program.cpp
  src/graphics/Shader.cpp
  src/graphics/ShaderLibrary.cpp
  src/graphics/Texture.cpp
  src/graphics/VertexArray.cpp
  src/platform/OpenGL/OpenGLContext.cpp
  src/platform/OpenGL/OpenGLRendererAPI.cpp
  src/platform/OpenGL/OpenGLWindow.cpp
  src/platform/Platform.cpp
  src/renderer/RendererAPI.cpp
  src/renderer/RendererContext.cpp
  src/renderer/Renderer.cpp
  src/renderer/Window.cpp
)
target_compile_options(redoom-core PRIVATE ${WARNING_FLAGS})
target_include_directories(redoom-core PUBLIC include)
set_property(TARGET redoom-core PROPERTY CXX_STANDARD 20)
set_property(TARGET redoom-core PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(
  redoom-core
  Threads::Threads
  redoom-utils
  fmt
  glfw
  glew
  glm
  stb
)

add_executable(
  redoom
  src/main.cpp
)
target_link_libraries(
  redoom
  redoom-core
)
target_link_options(redoom PRIVATE -rdynamic)
target_compile_options(redoom PRIVATE ${WARNING_FLAGS})
set_property(TARGET redoom PROPERTY CXX_STANDARD 20)
set_property(TARGET redoom PROPERTY CXX_STANDARD_REQUIRED ON)

if(REDOOM_BUILD_TESTS)
  add_subdirectory(tests)
endif()
if(REDOOM_BUILD_BENCHMARKS)
  add_subdirectory(benchmarks)
endif()
if(REDOOM_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
