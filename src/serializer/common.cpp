#include <redoom/serializer/common.hh>

namespace YAML
{
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
{
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq; // NOLINT
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
{
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq; // NOLINT
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
{
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq; // NOLINT
  return out;
}

Node convert<glm::vec2>::encode(const glm::vec2& rhs)
{
  Node node;
  node.push_back(rhs.x); // NOLINT
  node.push_back(rhs.y); // NOLINT
  node.SetStyle(EmitterStyle::Flow);
  return node;
}

bool convert<glm::vec2>::decode(const Node& node, glm::vec2& rhs)
{
  if (!node.IsSequence() || node.size() != 2)
    return false;

  rhs.x = node[0].as<float>(); // NOLINT
  rhs.y = node[1].as<float>(); // NOLINT
  return true;
}

Node convert<glm::vec3>::encode(const glm::vec3& rhs)
{
  Node node;
  node.push_back(rhs.x); // NOLINT
  node.push_back(rhs.y); // NOLINT
  node.push_back(rhs.z); // NOLINT
  node.SetStyle(EmitterStyle::Flow);
  return node;
}

bool convert<glm::vec3>::decode(const Node& node, glm::vec3& rhs)
{
  if (!node.IsSequence() || node.size() != 3)
    return false;

  rhs.x = node[0].as<float>(); // NOLINT
  rhs.y = node[1].as<float>(); // NOLINT
  rhs.z = node[2].as<float>(); // NOLINT
  return true;
}

Node convert<glm::vec4>::encode(const glm::vec4& rhs)
{
  Node node;
  node.push_back(rhs.x); // NOLINT
  node.push_back(rhs.y); // NOLINT
  node.push_back(rhs.z); // NOLINT
  node.push_back(rhs.w); // NOLINT
  node.SetStyle(EmitterStyle::Flow);
  return node;
}

bool convert<glm::vec4>::decode(const Node& node, glm::vec4& rhs)
{
  if (!node.IsSequence() || node.size() != 4)
    return false;

  rhs.x = node[0].as<float>(); // NOLINT
  rhs.y = node[1].as<float>(); // NOLINT
  rhs.z = node[2].as<float>(); // NOLINT
  rhs.w = node[3].as<float>(); // NOLINT
  return true;
}
} // namespace YAML
