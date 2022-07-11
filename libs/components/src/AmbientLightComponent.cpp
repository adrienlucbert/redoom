#include <components/extern.hh>

#include <redoom/ecs/components/AmbientLightComponent.hh>

using redoom::ecs::components::AmbientLightComponent;

redoom::Utils::type_id getTypeId() noexcept
{
  return AmbientLightComponent::getTypeId();
}

void serialize(YAML::Emitter& out, redoom::ecs::ComponentBase const* component)
{
  auto const* mc = dynamic_cast<AmbientLightComponent const*>(component);
  out << YAML::Key << "color" << YAML::Value << mc->color_;
}

redoom::Expected<> deserialize(
    YAML::Node const& node, redoom::Scene& scene, redoom::ecs::Entity entity)
{
  auto color = glm::vec3{1.0f, 1.0f, 1.0f};
  if (node["color"])
    color = node["color"].as<glm::vec3>();
  scene.getRegistry().attachComponent<AmbientLightComponent>(entity, color);
  return {};
}
