#include <redoom/ecs/EntityManager.hh>

#include <algorithm>
#include <atomic>

#include <redoom/memory/Allocator.hh>

namespace redoom::ecs
{
Entity EntityManager::make() noexcept
{
  static std::atomic<unsigned int> id = 0;
  auto entity = id++;
  auto lock = std::lock_guard{*this->mutex};
  this->entities.insert(
      std::upper_bound(this->entities.begin(), this->entities.end(), entity),
      entity);
  return entity;
}

void EntityManager::release(Entity entity) noexcept
{
  // TODO(alucbert): allow deleting a range (remove_if or so)
  auto lock = std::lock_guard{*this->mutex};
  auto range =
      std::equal_range(this->entities.begin(), this->entities.end(), entity);
  if (std::distance(range.first, range.second) == 1) {
    this->entities.erase(range.first, range.second);
  } else
    assert("Exactly one element should be released" == nullptr);
}

bool EntityManager::has(Entity entity) const noexcept
{
  return std::binary_search(
      this->entities.begin(), this->entities.end(), entity);
}

std::vector<Entity> const& EntityManager::getEntities() const noexcept
{
  return this->entities;
}
} // namespace redoom::ecs
