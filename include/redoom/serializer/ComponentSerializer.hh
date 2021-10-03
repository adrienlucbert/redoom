#pragma once

#include <yaml-cpp/yaml.h>

#include <Utils/Expected.hh>
#include <redoom/ecs/ComponentBase.hh>
#include <redoom/ecs/Entity.hh>

namespace redoom
{
class Scene;

struct ComponentSerializer {
  ComponentSerializer() noexcept = default;
  ComponentSerializer(ComponentSerializer const& b) noexcept = default;
  ComponentSerializer(ComponentSerializer&& b) noexcept = default;
  virtual ~ComponentSerializer() noexcept = default;

  ComponentSerializer& operator=(
      ComponentSerializer const& rhs) noexcept = default;
  ComponentSerializer& operator=(ComponentSerializer&& rhs) noexcept = default;

  virtual void serialize(
      YAML::Emitter& out, ecs::ComponentBase const* component) const = 0;
  [[nodiscard]] virtual Expected<> deserialize(
      YAML::Node const& node, Scene& scene, ecs::Entity entity) const = 0;
};
} // namespace redoom
