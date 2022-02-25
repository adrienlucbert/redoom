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

  TransformComponent(glm::vec3 pposition,
      float pangle,
      glm::vec3 protation,
      glm::vec3 pscale) noexcept
    : physics::BodyTransform{pposition, pangle, protation, pscale}
  {
  }

  struct Serializer : public ComponentSerializer {
    void serialize(
        YAML::Emitter& out, ecs::ComponentBase const* component) const override
    {
      auto const* tc = dynamic_cast<TransformComponent const*>(component);
      out << YAML::Key << "position" << YAML::Value << tc->position;
      out << YAML::Key << "angle" << YAML::Value << tc->angle;
      out << YAML::Key << "rotation" << YAML::Value << tc->rotation;
      out << YAML::Key << "scale" << YAML::Value << tc->scale;
    }
    [[nodiscard]] Expected<> deserialize(
        YAML::Node const& node, Scene& scene, Entity entity) const override
    {
      auto pposition = node["position"].as<glm::vec3>();
      auto pangle = node["angle"].as<float>();
      auto protation = node["rotation"].as<glm::vec3>();
      auto pscale = node["scale"].as<glm::vec3>();
      scene.getRegistry().attachComponent<TransformComponent>(
          entity, TransformComponent{pposition, pangle, protation, pscale});
      return {};
    }
  };
};
} // namespace redoom::ecs::components
