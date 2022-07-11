#include <meshes/extern.hh>

#include <redoom/graphics/mesh/Cuboid.hh>

using redoom::graphics::Mesh;
using redoom::graphics::mesh::Cuboid;

void serialize(YAML::Emitter& out, std::shared_ptr<Mesh> const& mesh)
{
  auto const* cuboid = dynamic_cast<Cuboid const*>(mesh.get());
  out << YAML::Key << "size" << YAML::Value << cuboid->size_;
  out << YAML::Key << "topology" << YAML::Value << cuboid->getTopology();
}

redoom::Expected<std::shared_ptr<Mesh>> deserialize(YAML::Node const& node)
{
  auto size = node["size"].as<glm::vec3>();
  auto topology = static_cast<GLenum>(GL_TRIANGLES);
  if (node["topology"])
    topology = node["topology"].as<GLenum>();
  return std::make_shared<Cuboid>(size.x,
      size.y,
      size.z,
      std::vector<redoom::graphics::Texture2D>{},
      topology);
}
