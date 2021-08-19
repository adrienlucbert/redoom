#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct EventSystem : public System<EventSystem> {
  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
