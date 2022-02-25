#pragma once

#include <map>
#include <memory>

#include <yaml-cpp/yaml.h>

#include <Utils/Expected.hh>
#include <Utils/Singleton.hh>
#include <Utils/UniqueTypeId.hh>
#include <redoom/serializer/ComponentSerializer.hh>

namespace redoom
{
class Scene;

struct SerializerPriority {
  unsigned int priority;

  bool operator<(SerializerPriority const& rhs) const noexcept
  {
    return this->priority < rhs.priority;
  }
};

class SceneSerializer : public Utils::Singleton<SceneSerializer>
{
public:
  [[nodiscard]] Expected<> serialize(
      std::string_view filepath, Scene const& scene) const noexcept;
  [[nodiscard]] Expected<> deserialize(
      Scene& scene, std::string_view filepath) const noexcept;

  template <typename T>
  void registerComponentFactory(std::string name,
      std::unique_ptr<ComponentSerializer> serializer,
      SerializerPriority priority = {0u}) noexcept
  {
    static_assert(std::is_base_of_v<ecs::ComponentBase, T>,
        "C must inherit from ComponentBase");
    assert(!this->find(name).has_value() && "Duplicate component factory");
    this->component_serializers.emplace(priority,
        SerializerData{T::getTypeId(), std::move(name), std::move(serializer)});
  }

protected:
  SceneSerializer() noexcept = default;

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

  [[nodiscard]] Expected<ComponentSerializer const*> find(
      std::string_view name) const noexcept;

  struct SerializerData {
    Utils::type_id type_id;
    std::string name;
    std::unique_ptr<ComponentSerializer> serializer;
  };

  std::multimap<SerializerPriority, SerializerData> component_serializers;
};
} // namespace redoom
