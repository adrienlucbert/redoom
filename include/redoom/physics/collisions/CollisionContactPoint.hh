#pragma once

#include <glm/gtc/type_ptr.hpp>

namespace redoom::physics
{
struct CollisionContactPoint {
  glm::vec3 position;
  glm::vec3 normal;
  float penetration;
};
} // namespace redoom::physics
