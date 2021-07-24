add_library(
  stb STATIC
  src/stb_image.cpp
)
target_include_directories(stb SYSTEM PUBLIC include)
