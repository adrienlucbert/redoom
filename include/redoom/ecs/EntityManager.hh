#pragma once

#include <memory>
#include <mutex>
#include <vector>

#include <redoom/ecs/Entity.hh>

namespace redoom::ecs
{
class EntityManager
{
public:
  EntityManager() noexcept = default;
  EntityManager(EntityManager const& b) noexcept = delete;
  EntityManager(EntityManager&& b) noexcept = default;
  ~EntityManager() noexcept = default;

  EntityManager& operator=(EntityManager const& rhs) noexcept = delete;
  EntityManager& operator=(EntityManager&& rhs) noexcept = default;

  [[nodiscard]] Entity make() noexcept;
  void release(Entity entity) noexcept;
  [[nodiscard]] bool has(Entity entity) const noexcept;

private:
  mutable std::unique_ptr<std::mutex> mutex{std::make_unique<std::mutex>()};
  std::vector<Entity> entities;
};
} // namespace redoom::ecs
