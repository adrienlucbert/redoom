#pragma once

#include <redoom/ecs/Component.hh>

#include <glm/vec3.hpp>

namespace redoom::ecs::components
{
struct TransformComponent : public Component<TransformComponent> {
  explicit TransformComponent(glm::vec3 pposition = {0.0f, 0.0f, 0.0f},
      float pangle = 0.0f,
      glm::vec3 protation = {1.0f, 1.0f, 1.0f},
      glm::vec3 pscale = {1.0f, 1.0f, 1.0f}) noexcept
    : position{pposition}
    , angle{pangle}
    , rotation{protation}
    , scale{pscale}
  {
  }

  glm::vec3 position{0.0f, 0.0f, 0.0f};
  float angle{0.0f};
  glm::vec3 rotation{0.0f, 0.0f, 0.0f};
  glm::vec3 scale{1.0f, 1.0f, 1.0f};
};
} // namespace redoom::ecs::components
