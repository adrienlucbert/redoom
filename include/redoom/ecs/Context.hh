#pragma once

#include <mutex>
#include <queue>

#include <redoom/ecs/ComponentManager.hh>
#include <redoom/ecs/EntityManager.hh>

namespace redoom::ecs
{
class Context
{
public:
  Context(ComponentManager& component_manager,
      EntityManager& entity_manager) noexcept;
  Context(Context const&) noexcept = delete;
  Context(Context&&) noexcept = delete;
  ~Context() noexcept;

  Context& operator=(Context const&) noexcept = delete;
  Context& operator=(Context&&) noexcept = delete;

  ComponentManager& getComponentManager() const noexcept;
  EntityManager& getEntityManager() const noexcept;

  void removeEntity(Entity entity) noexcept;

private:
  ComponentManager& component_manager_;
  EntityManager& entity_manager_;
  mutable std::mutex mutex_;
  std::queue<Entity> entity_delete_queue_;
};
} // namespace redoom::ecs
