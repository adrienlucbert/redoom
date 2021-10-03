#include <redoom/serializer/SceneSerializer.hh>

#include <fstream>
#include <iostream>

#include <fmt/format.h>
#include <glm/gtc/type_ptr.hpp>
#include <tl/expected.hpp>
#include <yaml-cpp/yaml.h>

#include <redoom/Scene.hh>

namespace redoom
{
void SceneSerializer::registerComponentFactory(std::string const& name,
    std::unique_ptr<ComponentSerializer> serializer) noexcept
{
  assert(this->component_serializers.find(name)
             == this->component_serializers.end()
         && "Duplicate component factory");
  this->component_serializers.emplace(name, std::move(serializer));
}

void SceneSerializer::serialize(
    std::string_view filepath, Scene const& scene) const noexcept
{
  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "Scene" << YAML::Value << scene.getName();
  out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

  for (auto const entity : scene.getRegistry().getEntities()) {
    out << YAML::BeginMap;
    for (auto const& component : scene.getRegistry().getComponents(entity)) {
      auto const type = component.get().getType();
      auto const& serializer_it = this->component_serializers.find(type);
      if (serializer_it == this->component_serializers.end()) {
        std::cerr << fmt::format(
            "Component type {} has no associated serializer", type)
                  << '\n';
      } else {
        out << YAML::Key << type << YAML::Value << YAML::BeginMap;
        serializer_it->second->serialize(out, &component.get());
        out << YAML::EndMap;
      }
    }
    out << YAML::EndMap;
  }
  out << YAML::EndSeq;
  out << YAML::EndMap;
  auto fout = std::ofstream{filepath.data()};
  fout << out.c_str();
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
        auto entity = scene.getRegistry().makeEntity();
        for (auto const& component_node : entity_node) {
          auto const type = component_node.first.as<std::string>();
          auto const& serializer_it = this->component_serializers.find(type);
          if (serializer_it == this->component_serializers.end()) {
            std::cerr << fmt::format(
                "Component type {} has no associated serializer", type)
                      << '\n';
          } else {
            auto exp = serializer_it->second->deserialize(
                component_node.second, scene, entity);
            RETURN_IF_UNEXPECTED(exp);
          }
        }
      }
    }
  } catch (YAML::Exception const& e) {
    return tl::make_unexpected(e.what());
  }
  return {};
}
} // namespace redoom
