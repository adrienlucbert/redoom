#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct BehaviourSystem : public System<BehaviourSystem> {
  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
