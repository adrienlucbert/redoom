#pragma once

#include <redoom/ComponentSerializer.hh>
#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/graphics/Camera.hh>

namespace redoom::ecs::components
{
struct CameraComponent : public Component<CameraComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"CameraComponent"};
    return type;
  }

  explicit CameraComponent(
      graphics::Camera pcamera = graphics::Camera{}) noexcept
    : camera{pcamera}
  {
  }
  graphics::Camera camera;

  struct Serializer : public ComponentSerializer {
    void serialize(YAML::Emitter& out,
        ecs::ComponentBase const* /*component*/) const noexcept override
    {
      out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap;
      // TODO(alucbert): dump camera
      out << YAML::EndMap;
    }
    [[nodiscard]] Expected<> deserialize(YAML::Node const& /*node*/,
        Scene& scene,
        Entity entity) const noexcept override
    {
      scene.getRegistry().attachComponent<CameraComponent>(
          entity, graphics::Camera{glm::vec3{0.0f, 2.0f, 5.0f}});
      return {};
    }
  };
};
} // namespace redoom::ecs::components
