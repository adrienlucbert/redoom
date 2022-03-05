#include <redoom/ecs/Registry.hh>

#include <redoom/ecs/Context.hh>
#include <redoom/ecs/UpdateContext.hh>
#include <redoom/ecs/components/BehaviourComponent.hh>

namespace redoom::ecs
{
Entity Registry::makeEntity() noexcept
{
  return this->entity_manager_.make();
}

void Registry::releaseEntity(Entity entity) noexcept
{
  this->entity_manager_.release(entity);
}

bool Registry::hasEntity(Entity entity) const noexcept
{
  return this->entity_manager_.has(entity);
}

void Registry::init() noexcept
{
  auto context = this->getContext();
  context.getComponentManager().apply<components::BehaviourComponent>(
      [&](auto entity, auto& component) { component.init(entity, context); });
  this->is_init_ = true;
}

void Registry::update(renderer::Window& window, double elapsed_time) noexcept
{
  if (!this->is_init_)
    this->init();
  auto context = this->getUpdateContext(window, elapsed_time);
  this->system_manager_.update(context);
}

Context Registry::getContext() noexcept
{
  return Context{this->component_manager_, this->entity_manager_};
}

UpdateContext Registry::getUpdateContext(
    renderer::Window& window, double elapsed_time) noexcept
{
  return UpdateContext{
      this->component_manager_, this->entity_manager_, elapsed_time, window};
}
} // namespace redoom::ecs
