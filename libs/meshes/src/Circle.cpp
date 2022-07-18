#include <meshes/extern.hh>

#include <redoom/graphics/mesh/Circle.hh>

using redoom::graphics::Mesh;
using redoom::graphics::mesh::Circle;

void serialize(YAML::Emitter& out, std::shared_ptr<Mesh> const& mesh)
{
  auto const* circle = dynamic_cast<Circle const*>(mesh.get());
  out << YAML::Key << "edge_segments" << YAML::Value << circle->edge_segments_;
  out << YAML::Key << "ring_segments" << YAML::Value << circle->ring_segments_;
}

redoom::Expected<std::shared_ptr<Mesh>> deserialize(YAML::Node const& node)
{
  auto edge_segments = node["edge_segments"].as<unsigned int>();
  auto ring_segments = node["ring_segments"].as<unsigned int>();
  return std::make_shared<Circle>(edge_segments, ring_segments);
}
