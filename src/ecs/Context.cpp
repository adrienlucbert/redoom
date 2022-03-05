#include <redoom/ecs/Context.hh>

namespace redoom::ecs
{
Context::Context(
    ComponentManager& component_manager, EntityManager& entity_manager) noexcept
  : component_manager_{component_manager}
  , entity_manager_{entity_manager}
{
}

Context::~Context() noexcept
{
  auto lock = std::lock_guard{this->mutex_};
  while (!this->entity_delete_queue_.empty()) {
    auto entity = this->entity_delete_queue_.front();
    this->entity_delete_queue_.pop();
    this->getComponentManager().releaseAll(entity);
    this->entity_manager_.release(entity);
  }
}

ComponentManager& Context::getComponentManager() const noexcept
{
  return this->component_manager_;
}

EntityManager& Context::getEntityManager() const noexcept
{
  return this->entity_manager_;
}

void Context::removeEntity(Entity entity) noexcept
{
  auto lock = std::lock_guard{this->mutex_};
  this->entity_delete_queue_.push(entity);
}
} // namespace redoom::ecs
