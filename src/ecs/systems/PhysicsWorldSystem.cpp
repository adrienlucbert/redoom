#include <redoom/ecs/systems/PhysicsWorldSystem.hh>

#include <redoom/Application.hh>

namespace redoom::ecs::systems
{
void PhysicsWorldSystem::update(UpdateContext& context) noexcept
{
  Application::get().getCurrentScene().getWorld().step(context.elapsed_time);
}
} // namespace redoom::ecs::systems
