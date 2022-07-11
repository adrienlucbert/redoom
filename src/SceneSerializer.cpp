#include <redoom/serializer/SceneSerializer.hh>

#include <filesystem>
#include <fstream>
#include <iostream>

#include <yaml-cpp/yaml.h>

#include <Utils/Expected.hh>
#include <redoom/Scene.hh>

namespace redoom
{
SceneSerializer::SceneSerializer() noexcept
{
  auto load_order_exp = this->loadSerializers();
  if (!load_order_exp)
    std::cerr << "Could not parse serialization order file: "
              << load_order_exp.error() << '\n';
}

Expected<> SceneSerializer::loadSerializers() noexcept
{
  YAML::Node data;
  try {
    auto filepath =
        std::filesystem::path{"../settings/SerializationOrder.yaml"};
    data = YAML::LoadFile(filepath.c_str());
    auto const& root = data["Components"];
    if (!root)
      return make_formatted_unexpected(
          "{}: Invalid file format", filepath.c_str());
    for (auto const& item : root) {
      auto component_type = item.first.as<std::string>();
      auto load_exp = this->factory_.loadSerializer(
          fmt::format("lib/lib{}.so", component_type), component_type);
      RETURN_IF_UNEXPECTED(load_exp);
      auto type_id_exp = this->factory_.getTypeId(component_type);
      RETURN_IF_UNEXPECTED(type_id_exp);
      this->component_serialization_order_.emplace(item.second.as<Priority>(),
          ComponentTypeData{*type_id_exp, std::move(component_type)});
    }
  } catch (YAML::Exception const& e) {
    return tl::make_unexpected(e.what());
  }
  return {};
}

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
  for (auto const& component_type : this->component_serialization_order_) {
    auto component_opt = scene.getRegistry().getComponentByTypeId(
        component_type.second.type_id, entity);
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
  out << YAML::Key << type << YAML::Value << YAML::BeginMap;
  try {
    this->factory_.serialize(type, out, &component.get());
  } catch (std::exception const& e) {
    std::cerr << "Serialization failed: " << e.what() << '\n';
  }
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
  return this->factory_.deserialize(type, component_node, scene, entity);
}
} // namespace redoom
