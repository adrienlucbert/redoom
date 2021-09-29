file(GLOB BEHAVIOURS_FILES src/**.cpp)
foreach(BEHAVIOUR_FILE ${BEHAVIOURS_FILES})
  get_filename_component(BEHAVIOUR_NAME ${BEHAVIOUR_FILE} NAME_WE)
  add_library(
    ${BEHAVIOUR_NAME} SHARED
    ${BEHAVIOUR_FILE}
  )
  target_include_directories(${BEHAVIOUR_NAME} PRIVATE include)
  target_compile_options(${BEHAVIOUR_NAME} PRIVATE ${WARNING_FLAGS})
  target_link_libraries(${BEHAVIOUR_NAME} PRIVATE redoom-core)
  set_property(TARGET ${BEHAVIOUR_NAME} PROPERTY CXX_STANDARD 20)
  set_property(TARGET ${BEHAVIOUR_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
endforeach()
