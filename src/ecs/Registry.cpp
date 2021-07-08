#include <redoom/ecs/Registry.hh>

namespace redoom::ecs
{
Entity& Registry::makeEntity() noexcept
{
  return this->entity_manager.make();
}

void Registry::releaseEntity(Entity& entity) noexcept
{
  this->entity_manager.release(entity);
}

void Registry::update(long elapsed_time) noexcept
{
  this->system_manager.update(elapsed_time, this->component_manager);
}
} // namespace redoom::ecs
