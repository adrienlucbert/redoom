#pragma once

#include <memory>
#include <unordered_map>

#include <yaml-cpp/yaml.h>

#include <Utils/Expected.hh>
#include <Utils/Singleton.hh>
#include <redoom/ComponentSerializer.hh>

namespace redoom
{
class Scene;

class SceneSerializer : public Utils::Singleton<SceneSerializer>
{
public:
  void serialize(std::string_view filepath, Scene const& scene) const noexcept;
  [[nodiscard]] Expected<> deserialize(
      Scene& scene, std::string_view filepath) const noexcept;

  void registerComponentFactory(std::string const& name,
      std::unique_ptr<ComponentSerializer> serializer) noexcept;

protected:
  SceneSerializer() noexcept = default;

  std::unordered_map<std::string, std::unique_ptr<ComponentSerializer>>
      component_serializers;
};
} // namespace redoom
