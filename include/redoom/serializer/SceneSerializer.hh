#pragma once

#include <map>
#include <memory>

#include <yaml-cpp/yaml.h>

#include <Utils/Expected.hh>
#include <Utils/Singleton.hh>
#include <Utils/UniqueTypeId.hh>
#include <redoom/serializer/ComponentFactory.hh>

namespace redoom
{
class Scene;

class SceneSerializer : public Utils::Singleton<SceneSerializer>
{
public:
  using Priority = unsigned int;

  [[nodiscard]] Expected<> serialize(
      std::string_view filepath, Scene const& scene) const noexcept;
  [[nodiscard]] Expected<> deserialize(
      Scene& scene, std::string_view filepath) const noexcept;

protected:
  SceneSerializer() noexcept;

  [[nodiscard]] Expected<> loadSerializers() noexcept;

  [[nodiscard]] Expected<> serializeEntity(
      YAML::Emitter& out, ecs::Entity entity, Scene const& scene) const;
  [[nodiscard]] Expected<> serializeComponent(YAML::Emitter& out,
      std::reference_wrapper<const ecs::ComponentBase> component) const;

  [[nodiscard]] Expected<> deserializeEntity(
      YAML::Node const& entity_node, Scene& scene) const;
  [[nodiscard]] Expected<> deserializeComponent(std::string_view type,
      YAML::Node const& component_node,
      ecs::Entity entity,
      Scene& scene) const;

  struct ComponentTypeData {
    Utils::type_id type_id;
    std::string name;
  };

  std::multimap<Priority, ComponentTypeData> component_serialization_order_{};
  ComponentFactory factory_;
};
} // namespace redoom
