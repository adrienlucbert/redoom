#include <mutex>
#include <redoom/ecs/EntityManager.hh>

#include <redoom/memory/Allocator.hh>

namespace redoom::ecs
{
Entity& EntityManager::makeEntity() noexcept
{
  auto* entity_p = static_cast<Entity*>(memory::Allocator<Entity>{}.get());
  this->mutex.lock();
  this->entities.emplace(entity_p->getId(), entity_p);
  this->mutex.unlock();
  return *this->entities.at(entity_p->getId());
}

void EntityManager::releaseEntity(Entity& entity) noexcept
{
  auto entity_it = this->entities.find(entity.getId());

  if (entity_it != this->entities.end()) {
    auto* entity_p = entity_it->second;
    memory::Allocator<Entity>{}.release(entity_p);
    this->mutex.lock();
    this->entities.erase(entity_it);
    this->mutex.unlock();
  }
}
} // namespace redoom::ecs
