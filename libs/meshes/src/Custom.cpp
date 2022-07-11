#include <meshes/extern.hh>

#include <redoom/graphics/Mesh.hh>

using redoom::graphics::Mesh;

void serialize(YAML::Emitter& /*out*/, std::shared_ptr<Mesh> const& /*mesh*/)
{
}

redoom::Expected<std::shared_ptr<Mesh>> deserialize(YAML::Node const& /*node*/)
{
  return tl::make_unexpected("Custom meshes are not supported yet");
}
