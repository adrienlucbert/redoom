#pragma once

#include <glm/gtc/type_ptr.hpp>

namespace redoom::physics
{
struct Force {
  enum class Type { Force, Acceleration, Impulse, VelocityChange };

  glm::vec3 force;
  Type type{Type::Force};

  [[nodiscard]] glm::vec3 compute(float mass, double timestep) const noexcept;
};
} // namespace redoom::physics
