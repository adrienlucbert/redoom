add_library(
  stb SHARED
  src/stb_image.cpp
)
target_include_directories(stb SYSTEM PUBLIC include)
