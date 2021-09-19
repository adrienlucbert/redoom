#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct CameraSystem : public MonothreadedSystem<CameraSystem> {
  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
