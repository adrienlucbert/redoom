#include <meshes/extern.hh>

#include <redoom/graphics/mesh/Torus.hh>

using redoom::graphics::Mesh;
using redoom::graphics::mesh::Torus;

void serialize(YAML::Emitter& out, std::shared_ptr<Mesh> const& mesh)
{
  auto const* torus = dynamic_cast<Torus const*>(mesh.get());
  out << YAML::Key << "r1" << YAML::Value << torus->r1_;
  out << YAML::Key << "r2" << YAML::Value << torus->r2_;
  out << YAML::Key << "num_steps1" << YAML::Value << torus->num_steps1_;
  out << YAML::Key << "num_steps2" << YAML::Value << torus->num_steps2_;
}

redoom::Expected<std::shared_ptr<Mesh>> deserialize(YAML::Node const& node)
{
  auto r1 = node["r1"].as<float>();
  auto r2 = node["r2"].as<float>();
  auto num_steps1 = node["num_steps1"].as<unsigned int>();
  auto num_steps2 = node["num_steps2"].as<unsigned int>();
  return std::make_shared<Torus>(r1, r2, num_steps1, num_steps2);
}
