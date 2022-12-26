#include <redoom/ecs/systems/PhysicsWorldSystem.hh>

#include <redoom/Runtime.hh>

namespace redoom::ecs::systems
{
void PhysicsWorldSystem::update(UpdateContext& context) noexcept
{
  auto& world = Runtime::get().getCurrentScene().getWorld();
  world.step(context.getElapsedTime());
}
} // namespace redoom::ecs::systems
