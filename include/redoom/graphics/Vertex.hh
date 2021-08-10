#pragma once

#include <glm/gtc/type_ptr.hpp>

namespace redoom::graphics
{
struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 color;
  glm::vec2 texUV;
};
} // namespace redoom::graphics
