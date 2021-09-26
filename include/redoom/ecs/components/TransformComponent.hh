#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <Utils/Expected.hh>
#include <redoom/ComponentSerializer.hh>
#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>

namespace redoom::ecs::components
{
struct TransformComponent : public Component<TransformComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"TransformComponent"};
    return type;
  }

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

  struct Serializer : public ComponentSerializer {
    void serialize(YAML::Emitter& out,
        ecs::ComponentBase const* component) const noexcept override
    {
      auto const* tc = dynamic_cast<TransformComponent const*>(component);
      out << YAML::Key << "position" << YAML::Value << tc->position;
      out << YAML::Key << "angle" << YAML::Value << tc->angle;
      out << YAML::Key << "rotation" << YAML::Value << tc->rotation;
      out << YAML::Key << "scale" << YAML::Value << tc->scale;
    }
    [[nodiscard]] Expected<> deserialize(YAML::Node const& node,
        Scene& scene,
        Entity entity) const noexcept override
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
