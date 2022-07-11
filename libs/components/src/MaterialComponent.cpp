#include <components/extern.hh>

#include <stdexcept>

#include <redoom/ecs/components/MaterialComponent.hh>
#include <redoom/graphics/Material.hh>

using redoom::ecs::components::MaterialComponent;
using redoom::graphics::Material;

redoom::Utils::type_id getTypeId() noexcept
{
  return MaterialComponent::getTypeId();
}

void serialize(YAML::Emitter& out, redoom::ecs::ComponentBase const* component)
{
  auto const* mc = dynamic_cast<MaterialComponent const*>(component);
  out << YAML::Key << "shader" << YAML::Value << mc->material.shader;
  out << YAML::Key << "gloss" << YAML::Value << mc->material.gloss;
  out << YAML::Key << "smoothness" << YAML::Value << mc->material.smoothness;
  out << YAML::Key << "metallicness" << YAML::Value
      << mc->material.metallicness;
  out << YAML::Key << "color" << YAML::Value << mc->material.color;
}

redoom::Expected<> deserialize(
    YAML::Node const& node, redoom::Scene& scene, redoom::ecs::Entity entity)
{
  auto shader = std::string{"lit"};
  if (node["shader"])
    shader = node["shader"].as<std::string>();
  auto gloss = 6.0f;
  if (node["gloss"])
    gloss = node["gloss"].as<float>();
  auto smoothness = 1.0f;
  if (node["smoothness"])
    smoothness = node["smoothness"].as<float>();
  auto metallicness = 1.0f;
  if (node["metallicness"])
    metallicness = node["metallicness"].as<float>();
  auto color = glm::vec3{1.0f, 1.0f, 1.0f};
  if (node["color"])
    color = node["color"].as<glm::vec3>();
  scene.getRegistry().attachComponent<MaterialComponent>(entity,
      Material{std::move(shader), gloss, smoothness, metallicness, color});
  return {};
}
