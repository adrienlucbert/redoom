#pragma once

#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/graphics/Camera.hh>
#include <redoom/serializer/ComponentSerializer.hh>

namespace redoom::ecs::components
{
struct CameraComponent : public Component<CameraComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"CameraComponent"};
    return type;
  }

  explicit CameraComponent(
      graphics::Camera camera = graphics::Camera{}) noexcept
    : camera_{camera}
  {
  }
  graphics::Camera camera_;

  struct Serializer : public ComponentSerializer {
    void serialize(
        YAML::Emitter& out, ecs::ComponentBase const* component) const override
    {
      auto const* cc = dynamic_cast<CameraComponent const*>(component);
      out << YAML::Key << "camera" << YAML::Value << cc->camera_;
    }
    [[nodiscard]] Expected<> deserialize(
        YAML::Node const& node, Scene& scene, Entity entity) const override
    {
      scene.getRegistry().attachComponent<CameraComponent>(
          entity, node["camera"].as<graphics::Camera>());
      return {};
    }
  };
};
} // namespace redoom::ecs::components
