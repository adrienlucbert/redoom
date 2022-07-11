file(GLOB COMPONENTS_FILES src/**.cpp)
foreach(COMPONENT_FILE ${COMPONENTS_FILES})
  get_filename_component(COMPONENT_NAME ${COMPONENT_FILE} NAME_WE)
  add_library(
    ${COMPONENT_NAME} SHARED
    ${COMPONENT_FILE}
  )
  target_include_directories(${COMPONENT_NAME} PRIVATE include)
  target_compile_options(${COMPONENT_NAME} PRIVATE ${WARNING_FLAGS})
  target_link_libraries(${COMPONENT_NAME} PRIVATE redoom-core)
  set_property(TARGET ${COMPONENT_NAME} PROPERTY CXX_STANDARD 20)
  set_property(TARGET ${COMPONENT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
endforeach()
