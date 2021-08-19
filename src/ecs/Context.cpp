#include <redoom/ecs/Context.hh>

namespace redoom::ecs
{
Context::Context(ComponentManager& pcomponent_manager,
    EntityManager& pentity_manager) noexcept
  : component_manager{pcomponent_manager}
  , entity_manager{pentity_manager}
{
}

Context::~Context() noexcept
{
  auto lock = std::lock_guard{this->mutex};
  while (!this->entity_delete_queue.empty()) {
    auto entity = this->entity_delete_queue.front();
    this->entity_delete_queue.pop();
    this->component_manager.releaseAll(entity);
    this->entity_manager.release(entity);
  }
}

void Context::removeEntity(Entity entity) noexcept
{
  auto lock = std::lock_guard{this->mutex};
  this->entity_delete_queue.push(entity);
}
} // namespace redoom::ecs
