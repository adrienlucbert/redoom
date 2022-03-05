#include <redoom/ecs/systems/BehaviourSystem.hh>

#include <iostream>

#include <redoom/ecs/Registry.hh>
#include <redoom/ecs/components/BehaviourComponent.hh>

namespace redoom::ecs::systems
{
void BehaviourSystem::update(UpdateContext& context) noexcept
{
  context.getComponentManager().apply<components::BehaviourComponent>(
      [&](auto entity, auto& component) {
        auto behaviour_exp = component.template get();
        if (!behaviour_exp)
          std::cerr << behaviour_exp.error() << '\n';
        else {
          auto& behaviour = *behaviour_exp;
          behaviour.get().onUpdate(entity, context);
        }
      });
}
} // namespace redoom::ecs::systems
