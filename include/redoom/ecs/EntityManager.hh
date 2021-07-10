#pragma once

#include <mutex>
#include <unordered_map>

#include <redoom/ecs/Entity.hh>
#include <redoom/memory/Allocator.hh>

namespace redoom::ecs
{
using memory::Allocator;

class EntityManager
{
public:
  EntityManager() noexcept = default;
  EntityManager(EntityManager const& b) noexcept = delete;
  EntityManager(EntityManager&& b) noexcept = default;
  ~EntityManager() noexcept = default;

  EntityManager& operator=(EntityManager const& rhs) noexcept = delete;
  EntityManager& operator=(EntityManager&& rhs) noexcept = default;

  [[nodiscard]] Entity& make() noexcept;
  void release(Entity& entity) noexcept;
  [[nodiscard]] bool has(Entity const& entity) const noexcept;

private:
  mutable std::unique_ptr<std::mutex> mutex{std::make_unique<std::mutex>()};
  Allocator<Entity> allocator;
  std::unordered_map<unsigned int, Allocator<Entity>::ptr_t> entities;
};
} // namespace redoom::ecs
