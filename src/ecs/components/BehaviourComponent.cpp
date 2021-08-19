#include <redoom/ecs/components/BehaviourComponent.hh>

namespace redoom::ecs::components
{
BehaviourComponent::BehaviourComponent(
    std::unique_ptr<Behaviour> pbehaviour) noexcept
  : behaviour{std::move(pbehaviour)}
{
}

void BehaviourComponent::onInit(Entity entity, Context& context) noexcept
{
  this->get()->get().onInit(entity, context);
}

void BehaviourComponent::onDestroy(Entity entity, Context& context) noexcept
{
  this->get()->get().onDestroy(entity, context);
}
} // namespace redoom::ecs::components
