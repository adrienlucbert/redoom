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
  out << YAML::Key << "shader" << YAML::Value << mc->material.shader_;
  out << YAML::Key << "gloss" << YAML::Value << mc->material.gloss_;
  out << YAML::Key << "smoothness" << YAML::Value << mc->material.smoothness_;
  out << YAML::Key << "metallicness" << YAML::Value
      << mc->material.metallicness_;
  out << YAML::Key << "color" << YAML::Value << mc->material.color_;
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
  auto topology = static_cast<GLenum>(GL_TRIANGLES);
  if (node["topology"])
    topology = node["topology"].as<unsigned int>();
  scene.getRegistry().attachComponent<MaterialComponent>(entity,
      Material{std::move(shader),
          gloss,
          smoothness,
          metallicness,
          color,
          {},
          topology});
  return {};
}
