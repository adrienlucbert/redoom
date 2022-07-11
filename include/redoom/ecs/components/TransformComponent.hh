#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <Utils/Expected.hh>
#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/physics/Body.hh>

namespace redoom::ecs::components
{
struct TransformComponent : public Component<TransformComponent>,
                            public physics::BodyTransform {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"TransformComponent"};
    return type;
  }

  TransformComponent(glm::vec3 position,
      float angle,
      glm::vec3 rotation,
      glm::vec3 scale) noexcept
    : physics::BodyTransform{{position, angle, rotation, scale}}
  {
  }
};
} // namespace redoom::ecs::components
