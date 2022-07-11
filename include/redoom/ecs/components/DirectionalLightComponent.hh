#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>

namespace redoom::ecs::components
{
struct DirectionalLightComponent : public Component<DirectionalLightComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"DirectionalLightComponent"};
    return type;
  }

  explicit DirectionalLightComponent(
      glm::vec3 direction, glm::vec3 color = glm::vec3{1.0f, 1.0f, 1.0f})
    : direction_{direction}
    , color_{color}
  {
  }

  glm::vec3 direction_;
  glm::vec3 color_;
};
} // namespace redoom::ecs::components
