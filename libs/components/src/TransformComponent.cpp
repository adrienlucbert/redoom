#include <components/extern.hh>

#include <stdexcept>

#include <redoom/ecs/components/TransformComponent.hh>

using redoom::ecs::components::TransformComponent;

redoom::Utils::type_id getTypeId() noexcept
{
  return TransformComponent::getTypeId();
}

void serialize(YAML::Emitter& out, redoom::ecs::ComponentBase const* component)
{
  auto const* tc = dynamic_cast<TransformComponent const*>(component);
  out << YAML::Key << "position" << YAML::Value << tc->getPosition();
  out << YAML::Key << "angle" << YAML::Value << tc->getAngle();
  out << YAML::Key << "rotation" << YAML::Value << tc->getRotation();
  out << YAML::Key << "scale" << YAML::Value << tc->getScale();
}

redoom::Expected<> deserialize(
    YAML::Node const& node, redoom::Scene& scene, redoom::ecs::Entity entity)
{
  auto position = node["position"].as<glm::vec3>();
  auto angle = node["angle"].as<float>();
  auto rotation = node["rotation"].as<glm::vec3>();
  auto scale = node["scale"].as<glm::vec3>();
  scene.getRegistry().attachComponent<TransformComponent>(
      entity, TransformComponent{position, angle, rotation, scale});
  return {};
}
