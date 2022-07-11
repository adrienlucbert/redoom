#include <components/extern.hh>

#include <stdexcept>

#include <redoom/ecs/components/DirectionalLightComponent.hh>

using redoom::ecs::components::DirectionalLightComponent;

redoom::Utils::type_id getTypeId() noexcept
{
  return DirectionalLightComponent::getTypeId();
}

void serialize(YAML::Emitter& out, redoom::ecs::ComponentBase const* component)
{
  auto const* dc = dynamic_cast<DirectionalLightComponent const*>(component);
  out << YAML::Key << "direction" << YAML::Value << dc->direction_;
  out << YAML::Key << "color" << YAML::Value << dc->color_;
}

redoom::Expected<> deserialize(
    YAML::Node const& node, redoom::Scene& scene, redoom::ecs::Entity entity)
{
  auto direction = node["direction"].as<glm::vec3>();
  auto color = glm::vec3{1.0f, 1.0f, 1.0f};
  if (node["color"])
    color = node["color"].as<glm::vec3>();
  scene.getRegistry().attachComponent<DirectionalLightComponent>(
      entity, direction, color);
  return {};
}
