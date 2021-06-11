#include <redoom/ecs/EntityManager.hh>

#include <algorithm>

#include <redoom/memory/Allocator.hh>

namespace redoom::ecs
{
Entity& EntityManager::make() noexcept
{
  auto entity = this->allocator.get();
  auto lock = std::lock_guard{this->mutex};
  auto const& entity_it =
      this->entities.emplace(entity->getId(), std::move(entity)).first;
  return *entity_it->second;
}

void EntityManager::release(Entity& entity) noexcept
{
  auto entity_it = this->entities.find(entity.getId());

  if (entity_it != this->entities.end()) {
    auto lock = std::lock_guard{this->mutex};
    this->entities.erase(entity_it);
  }
}
} // namespace redoom::ecs
