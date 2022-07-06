#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/serializer/ComponentSerializer.hh>
#include <redoom/serializer/common.hh>

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

  struct Serializer : public ComponentSerializer {
    void serialize(
        YAML::Emitter& out, ecs::ComponentBase const* component) const override
    {
      auto const* mc = dynamic_cast<AmbientLightComponent const*>(component);
      out << YAML::Key << "color" << YAML::Value << mc->color_;
    }

    [[nodiscard]] Expected<> deserialize(
        YAML::Node const& node, Scene& scene, Entity entity) const override
    {
      auto color = glm::vec3{1.0f, 1.0f, 1.0f};
      if (node["color"])
        color = node["color"].as<glm::vec3>();
      scene.getRegistry().attachComponent<AmbientLightComponent>(entity, color);
      return {};
    }
  };
};
} // namespace redoom::ecs::components
