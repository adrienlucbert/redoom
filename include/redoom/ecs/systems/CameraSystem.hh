#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct CameraSystem : public System<CameraSystem> {
  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
