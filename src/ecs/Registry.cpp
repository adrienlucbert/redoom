#include <redoom/ecs/Registry.hh>

#include <redoom/ecs/UpdateContext.hh>

namespace redoom::ecs
{
Entity Registry::makeEntity() noexcept
{
  return this->entity_manager.make();
}

void Registry::releaseEntity(Entity entity) noexcept
{
  this->entity_manager.release(entity);
}

bool Registry::hasEntity(Entity entity) const noexcept
{
  return this->entity_manager.has(entity);
}

void Registry::update(long elapsed_time) noexcept
{
  auto context = UpdateContext{
      elapsed_time, this->component_manager, this->entity_manager};
  this->system_manager.update(context);
}
} // namespace redoom::ecs
