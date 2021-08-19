#pragma once

#include <mutex>
#include <queue>

#include <redoom/ecs/ComponentManager.hh>
#include <redoom/ecs/EntityManager.hh>

namespace redoom::ecs
{
struct Context {
  Context(ComponentManager& pcomponent_manager,
      EntityManager& pentity_manager) noexcept;
  Context(Context const&) noexcept = delete;
  Context(Context&&) noexcept = delete;
  ~Context() noexcept;

  Context& operator=(Context const&) noexcept = delete;
  Context& operator=(Context&&) noexcept = delete;

  void removeEntity(Entity entity) noexcept;

  ComponentManager& component_manager;
  EntityManager& entity_manager;

private:
  mutable std::mutex mutex;
  std::queue<Entity> entity_delete_queue;
};
} // namespace redoom::ecs
