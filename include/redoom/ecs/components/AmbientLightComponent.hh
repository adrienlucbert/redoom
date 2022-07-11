#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>

namespace redoom::ecs::components
{
struct AmbientLightComponent : public Component<AmbientLightComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"AmbientLightComponent"};
    return type;
  }

  explicit AmbientLightComponent(glm::vec3 color = glm::vec3{1.0f, 1.0f, 1.0f})
    : color_{color}
  {
  }

  glm::vec3 color_;
};
} // namespace redoom::ecs::components
