#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <yaml-cpp/yaml.h>

#include <Utils/Expected.hh>
#include <redoom/ecs/ComponentBase.hh>
#include <redoom/ecs/Entity.hh>

namespace YAML
{
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v);

template <>
struct convert<glm::vec3> {
  static Node encode(const glm::vec3& rhs);
  static bool decode(const Node& node, glm::vec3& rhs);
};

template <>
struct convert<glm::vec4> {
  static Node encode(const glm::vec4& rhs);
  static bool decode(const Node& node, glm::vec4& rhs);
};
} // namespace YAML

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

  virtual void serialize(YAML::Emitter& out,
      ecs::ComponentBase const* component) const noexcept = 0;
  [[nodiscard]] virtual Expected<> deserialize(YAML::Node const& node,
      Scene& scene,
      ecs::Entity entity) const noexcept = 0;
};
} // namespace redoom
