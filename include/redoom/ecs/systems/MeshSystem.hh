#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct MeshSystem : public System<MeshSystem> {
  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
