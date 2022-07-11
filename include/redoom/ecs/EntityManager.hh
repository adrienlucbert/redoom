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
  EntityManager(EntityManager const&) noexcept = delete;
  EntityManager(EntityManager&&) noexcept = default;
  ~EntityManager() noexcept = default;

  EntityManager& operator=(EntityManager const&) noexcept = delete;
  EntityManager& operator=(EntityManager&&) noexcept = default;

  [[nodiscard]] Entity make() noexcept;
  void release(Entity entity) noexcept;
  [[nodiscard]] bool has(Entity entity) const noexcept;

  [[nodiscard]] std::vector<Entity> const& getEntities() const noexcept;

private:
  mutable std::unique_ptr<std::mutex> mutex_{std::make_unique<std::mutex>()};
  std::vector<Entity> entities_;
};
} // namespace redoom::ecs
