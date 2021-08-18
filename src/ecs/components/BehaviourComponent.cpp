#include <redoom/ecs/components/BehaviourComponent.hh>

namespace redoom::ecs::components
{
BehaviourComponent::BehaviourComponent(
    std::unique_ptr<Behaviour> pbehaviour) noexcept
  : behaviour{std::move(pbehaviour)}
{
}
} // namespace redoom::ecs::components
