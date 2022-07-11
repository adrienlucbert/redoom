#include <components/extern.hh>

#include <stdexcept>

#include <redoom/ecs/components/CameraComponent.hh>
#include <redoom/graphics/Camera.hh>

using redoom::ecs::components::CameraComponent;
using redoom::graphics::Camera;

redoom::Utils::type_id getTypeId() noexcept
{
  return CameraComponent::getTypeId();
}

void serialize(YAML::Emitter& out, redoom::ecs::ComponentBase const* component)
{
  auto const* cc = dynamic_cast<CameraComponent const*>(component);
  out << YAML::Key << "camera" << YAML::Value << cc->camera_;
}

redoom::Expected<> deserialize(
    YAML::Node const& node, redoom::Scene& scene, redoom::ecs::Entity entity)
{
  scene.getRegistry().attachComponent<CameraComponent>(
      entity, node["camera"].as<Camera>());
  return {};
}
