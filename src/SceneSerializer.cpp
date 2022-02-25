#include <redoom/serializer/SceneSerializer.hh>

#include <fstream>

#include <yaml-cpp/yaml.h>

#include <Utils/Expected.hh>
#include <redoom/Scene.hh>

namespace redoom
{
Expected<> SceneSerializer::serialize(
    std::string_view filepath, Scene const& scene) const noexcept
{
  auto fout = std::ofstream{};
  // enable exceptions on ofstream error
  fout.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  try {
    fout.open(filepath.data());
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << scene.getName();
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

    for (auto const entity : scene.getRegistry().getEntities()) {
      RETURN_IF_UNEXPECTED(this->serializeEntity(out, entity, scene));
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;
    fout << out.c_str();
  } catch (std::ofstream::failure const& e) {
    return tl::unexpected(e.what());
  }
  return {};
}

Expected<> SceneSerializer::serializeEntity(
    YAML::Emitter& out, ecs::Entity entity, Scene const& scene) const
{
  out << YAML::BeginMap;
  for (auto const& component_serializer : this->component_serializers) {
    auto component_opt = scene.getRegistry().getComponentByTypeId(
        component_serializer.second.type_id, entity);
    if (component_opt.has_value())
      RETURN_IF_UNEXPECTED(this->serializeComponent(out, *component_opt));
  }
  out << YAML::EndMap;
  return {};
}

Expected<> SceneSerializer::serializeComponent(YAML::Emitter& out,
    std::reference_wrapper<const ecs::ComponentBase> component) const
{
  auto const type = component.get().getType();
  auto serializer_exp = this->find(type);
  RETURN_IF_UNEXPECTED(serializer_exp);
  out << YAML::Key << type << YAML::Value << YAML::BeginMap;
  serializer_exp.value()->serialize(out, &component.get());
  out << YAML::EndMap;
  return {};
}

Expected<> SceneSerializer::deserialize(
    Scene& scene, std::string_view filepath) const noexcept
{
  YAML::Node data;
  try {
    data = YAML::LoadFile(filepath.data());
    if (!data["Scene"])
      return make_formatted_unexpected("{}: Invalid scene file", filepath);
    auto const scene_name = data["Scene"].as<std::string>();
    scene.setName(scene_name);
    auto const entities = data["Entities"];
    if (entities) {
      for (auto const& entity_node : entities) {
        RETURN_IF_UNEXPECTED(this->deserializeEntity(entity_node, scene));
      }
    }
  } catch (YAML::Exception const& e) {
    return tl::make_unexpected(e.what());
  }
  return {};
}

Expected<> SceneSerializer::deserializeEntity(
    YAML::Node const& entity_node, Scene& scene) const
{
  auto entity = scene.getRegistry().makeEntity();
  for (auto const& component_it : entity_node) {
    RETURN_IF_UNEXPECTED(
        this->deserializeComponent(component_it.first.as<std::string>(),
            component_it.second,
            entity,
            scene));
  }
  return {};
}

Expected<> SceneSerializer::deserializeComponent(std::string_view type,
    YAML::Node const& component_node,
    ecs::Entity entity,
    Scene& scene) const
{
  auto serializer_exp = this->find(type);
  RETURN_IF_UNEXPECTED(serializer_exp);
  RETURN_IF_UNEXPECTED(
      serializer_exp.value()->deserialize(component_node, scene, entity));
  return {};
}

Expected<ComponentSerializer const*> SceneSerializer::find(
    std::string_view name) const noexcept
{
  auto it = std::find_if(this->component_serializers.begin(),
      this->component_serializers.end(),
      [&name](auto const& pair) { return pair.second.name == name; });
  if (it != this->component_serializers.end())
    return it->second.serializer.get();
  return make_formatted_unexpected(
      "Component type {} has no associated serializer", name);
}
} // namespace redoom
