#include <meshes/extern.hh>

#include <redoom/graphics/mesh/Sphere.hh>

using redoom::graphics::Mesh;
using redoom::graphics::mesh::Sphere;

void serialize(YAML::Emitter& out, std::shared_ptr<Mesh> const& mesh)
{
  auto const* sphere = dynamic_cast<Sphere const*>(mesh.get());
  out << YAML::Key << "radius" << YAML::Value << sphere->radius_;
  out << YAML::Key << "x_segment" << YAML::Value << sphere->x_segment_;
  out << YAML::Key << "y_segment" << YAML::Value << sphere->y_segment_;
  out << YAML::Key << "topology" << YAML::Value << sphere->getTopology();
}

redoom::Expected<std::shared_ptr<Mesh>> deserialize(YAML::Node const& node)
{
  auto radius = node["radius"].as<float>();
  auto x_segment = 20u;
  if (node["x_segment"])
    x_segment = node["x_segment"].as<unsigned int>();
  auto y_segment = 20u;
  if (node["y_segment"])
    y_segment = node["y_segment"].as<unsigned int>();
  auto topology = static_cast<GLenum>(GL_TRIANGLES);
  if (node["topology"])
    topology = node["topology"].as<GLenum>();
  return std::make_shared<Sphere>(radius,
      x_segment,
      y_segment,
      std::vector<redoom::graphics::Texture2D>{},
      topology);
}
