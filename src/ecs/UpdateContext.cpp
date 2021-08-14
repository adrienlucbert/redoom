#include <redoom/ecs/UpdateContext.hh>

namespace redoom::ecs
{
UpdateContext::UpdateContext(long pelapsed_time,
    ComponentManager& pcomponent_manager,
    EntityManager& pentity_manager) noexcept
  : elapsed_time{pelapsed_time}
  , component_manager{pcomponent_manager}
  , entity_manager{pentity_manager}
{
}

UpdateContext::~UpdateContext() noexcept
{
  while (!this->entity_delete_queue.empty()) {
    auto entity = this->entity_delete_queue.front();
    this->entity_delete_queue.pop();
    this->component_manager.releaseAll(entity);
    this->entity_manager.release(entity);
  }
}

void UpdateContext::removeEntity(Entity entity) noexcept
{
  auto lock = std::lock_guard{this->mutex};
  this->entity_delete_queue.push(entity);
}
} // namespace redoom::ecs
