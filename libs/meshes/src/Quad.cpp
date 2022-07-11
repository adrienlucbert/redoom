#include <meshes/extern.hh>

#include <redoom/graphics/mesh/Quad.hh>

using redoom::graphics::Mesh;
using redoom::graphics::mesh::Quad;

void serialize(YAML::Emitter& out, std::shared_ptr<Mesh> const& mesh)
{
  auto const* quad = dynamic_cast<Quad const*>(mesh.get());
  out << YAML::Key << "size" << YAML::Value << quad->size_;
  out << YAML::Key << "topology" << YAML::Value << quad->getTopology();
}

redoom::Expected<std::shared_ptr<Mesh>> deserialize(YAML::Node const& node)
{
  auto size = node["size"].as<glm::vec2>();
  auto topology = static_cast<GLenum>(GL_TRIANGLES);
  if (node["topology"])
    topology = node["topology"].as<GLenum>();
  return std::make_shared<Quad>(
      size.x, size.y, std::vector<redoom::graphics::Texture2D>{}, topology);
}
