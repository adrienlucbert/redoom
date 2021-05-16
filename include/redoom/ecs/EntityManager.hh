#pragma once

#include <redoom/ecs/Entity.hh>

#include <mutex>
#include <unordered_map>

namespace redoom::ecs
{
class EntityManager
{
public:
  EntityManager() noexcept = default;
  EntityManager(EntityManager const& b) noexcept = delete;
  EntityManager(EntityManager&& b) noexcept = delete;
  ~EntityManager() noexcept = default;

  EntityManager& operator=(EntityManager const& rhs) noexcept = delete;
  EntityManager& operator=(EntityManager&& rhs) noexcept = delete;

  [[nodiscard]] Entity& make() noexcept;
  void release(Entity& entity) noexcept;

private:
  mutable std::mutex mutex;
  std::unordered_map<unsigned int, Entity*> entities;
};
} // namespace redoom::ecs
