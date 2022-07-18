add_library(
  stb SHARED
  src/stb_image.cpp
  src/stb_image_write.cpp
)
target_include_directories(stb SYSTEM PUBLIC include)
