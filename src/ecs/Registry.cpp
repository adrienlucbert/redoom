#include <redoom/ecs/Registry.hh>

#include <redoom/ecs/Context.hh>
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

void Registry::init() noexcept
{
  auto context = this->getContext();
  context.component_manager.apply<components::BehaviourComponent>(
      [&](auto entity, auto& component) { component.init(entity, context); });
  this->is_init = true;
}

void Registry::update(renderer::Window& window, double elapsed_time) noexcept
{
  if (!this->is_init)
    this->init();
  auto context = this->getUpdateContext(window, elapsed_time);
  this->system_manager.update(context);
}

Context Registry::getContext() noexcept
{
  return Context{this->component_manager, this->entity_manager};
}

UpdateContext Registry::getUpdateContext(
    renderer::Window& window, double elapsed_time) noexcept
{
  return UpdateContext{
      this->component_manager, this->entity_manager, elapsed_time, window};
}
} // namespace redoom::ecs
