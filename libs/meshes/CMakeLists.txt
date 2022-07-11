file(GLOB MESHES_FILES src/**.cpp)
foreach(MESH_FILE ${MESHES_FILES})
  get_filename_component(MESH_NAME ${MESH_FILE} NAME_WE)
  add_library(
    ${MESH_NAME} SHARED
    ${MESH_FILE}
  )
  target_include_directories(${MESH_NAME} PRIVATE include)
  target_compile_options(${MESH_NAME} PRIVATE ${WARNING_FLAGS})
  target_link_libraries(${MESH_NAME} PRIVATE redoom-core)
  set_property(TARGET ${MESH_NAME} PROPERTY CXX_STANDARD 20)
  set_property(TARGET ${MESH_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
endforeach()
