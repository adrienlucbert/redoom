#include <redoom/ecs/systems/PhysicsWorldSystem.hh>

#include <redoom/Application.hh>

namespace redoom::ecs::systems
{
void PhysicsWorldSystem::update(UpdateContext& context) noexcept
{
  auto& world = Application::get().getCurrentScene().getWorld();
  world.step(context.elapsed_time);
}
} // namespace redoom::ecs::systems
