#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/serializer/ComponentSerializer.hh>
#include <redoom/serializer/common.hh>

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

  struct Serializer : public ComponentSerializer {
    void serialize(
        YAML::Emitter& out, ecs::ComponentBase const* component) const override
    {
      auto const* dc =
          dynamic_cast<DirectionalLightComponent const*>(component);
      out << YAML::Key << "direction" << YAML::Value << dc->direction_;
      out << YAML::Key << "color" << YAML::Value << dc->color_;
    }

    [[nodiscard]] Expected<> deserialize(
        YAML::Node const& node, Scene& scene, Entity entity) const override
    {
      auto direction = node["direction"].as<glm::vec3>();
      auto color = glm::vec3{1.0f, 1.0f, 1.0f};
      if (node["color"])
        color = node["color"].as<glm::vec3>();
      scene.getRegistry().attachComponent<DirectionalLightComponent>(
          entity, direction, color);
      return {};
    }
  }; // namespace redoom::ecs::components
};
} // namespace redoom::ecs::components
