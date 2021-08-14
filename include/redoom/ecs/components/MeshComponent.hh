#include <redoom/ecs/Component.hh>
#include <redoom/graphics/Mesh.hh>

namespace redoom::ecs::components
{
struct MeshComponent : public Component<MeshComponent> {
  graphics::Mesh mesh;
};
} // namespace redoom::ecs::components
