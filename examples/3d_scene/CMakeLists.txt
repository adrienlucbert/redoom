set(EXAMPLE_NAME 3d_scene)

add_executable(${EXAMPLE_NAME} src/main.cpp)
target_include_directories(${EXAMPLE_NAME} PUBLIC include)
target_link_libraries(
  ${EXAMPLE_NAME}
  redoom-core
)
target_compile_options(${EXAMPLE_NAME} PRIVATE ${WARNING_FLAGS})
set_property(TARGET ${EXAMPLE_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${EXAMPLE_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

add_subdirectory(behaviours)
