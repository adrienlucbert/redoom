#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <Utils/Expected.hh>
#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/physics/Body.hh>
#include <redoom/serializer/ComponentSerializer.hh>
#include <redoom/serializer/common.hh>

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
    : physics::BodyTransform{position, angle, rotation, scale}
  {
  }

  struct Serializer : public ComponentSerializer {
    void serialize(
        YAML::Emitter& out, ecs::ComponentBase const* component) const override
    {
      auto const* tc = dynamic_cast<TransformComponent const*>(component);
      out << YAML::Key << "position" << YAML::Value << tc->getPosition();
      out << YAML::Key << "angle" << YAML::Value << tc->getAngle();
      out << YAML::Key << "rotation" << YAML::Value << tc->getRotation();
      out << YAML::Key << "scale" << YAML::Value << tc->getScale();
    }
    [[nodiscard]] Expected<> deserialize(
        YAML::Node const& node, Scene& scene, Entity entity) const override
    {
      auto position = node["position"].as<glm::vec3>();
      auto angle = node["angle"].as<float>();
      auto rotation = node["rotation"].as<glm::vec3>();
      auto scale = node["scale"].as<glm::vec3>();
      scene.getRegistry().attachComponent<TransformComponent>(
          entity, TransformComponent{position, angle, rotation, scale});
      return {};
    }
  };
};
} // namespace redoom::ecs::components
