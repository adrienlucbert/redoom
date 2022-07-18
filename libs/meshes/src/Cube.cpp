#include <meshes/extern.hh>

#include <redoom/graphics/mesh/Cube.hh>

using redoom::graphics::Mesh;
using redoom::graphics::mesh::Cube;

void serialize(YAML::Emitter& out, std::shared_ptr<Mesh> const& mesh)
{
  auto const* cube = dynamic_cast<Cube const*>(mesh.get());
  out << YAML::Key << "size" << YAML::Value << cube->size_;
}

redoom::Expected<std::shared_ptr<Mesh>> deserialize(YAML::Node const& node)
{
  auto width = node["width"].as<float>();
  return std::make_shared<Cube>(width);
}
