#include <redoom/ecs/components/BehaviourComponent.hh>

#include <Utils/Expected.hh>
#include <redoom/Scene.hh>
#include <redoom/ecs/Behaviour.hh>

namespace redoom::ecs::components
{
BehaviourComponent::BehaviourComponent(
    std::unique_ptr<Behaviour> behaviour) noexcept
  : behaviour_{std::move(behaviour)}
{
}

std::string const& BehaviourComponent::getType() const noexcept
{
  static auto const type = std::string{"BehaviourComponent"};
  return type;
}
} // namespace redoom::ecs::components
