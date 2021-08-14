#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
class BehaviourSystem : public System<BehaviourSystem>
{
public:
  BehaviourSystem() noexcept = default;
  BehaviourSystem(BehaviourSystem const& b) noexcept = delete;
  BehaviourSystem(BehaviourSystem&& b) noexcept = default;
  ~BehaviourSystem() noexcept override = default;

  BehaviourSystem& operator=(BehaviourSystem const& rhs) noexcept = delete;
  BehaviourSystem& operator=(BehaviourSystem&& rhs) noexcept = default;

  void update(UpdateContext& context) noexcept override;

private:
};
} // namespace redoom::ecs::systems
