#include <memory>

#include <redoom/ecs/Component.hh>
#include <redoom/graphics/Mesh.hh>

namespace redoom::ecs::components
{
struct MeshComponent : public Component<MeshComponent> {
  explicit MeshComponent(std::shared_ptr<graphics::Mesh> pmesh)
    : mesh{std::move(pmesh)}
  {
  }

  std::shared_ptr<graphics::Mesh> mesh;
};
} // namespace redoom::ecs::components
