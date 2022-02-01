#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <yaml-cpp/yaml.h>

#include <Utils/Expected.hh>

namespace YAML
{
template <typename T>
[[nodiscard]] inline redoom::Expected<T> exp_get_value(
    YAML::Node const& node, std::string const& key) noexcept
{
  if (!node[key])
    return redoom::make_formatted_unexpected("Key missing: {}", key);
  return node[key].as<T>();
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v);

template <>
struct convert<glm::vec2> {
  static Node encode(const glm::vec2& rhs);
  static bool decode(const Node& node, glm::vec2& rhs);
};

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
