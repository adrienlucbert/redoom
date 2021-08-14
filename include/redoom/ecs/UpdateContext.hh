#pragma once

#include <mutex>
#include <queue>

#include <redoom/ecs/ComponentManager.hh>
#include <redoom/ecs/EntityManager.hh>

namespace redoom::ecs
{
struct UpdateContext {
  UpdateContext(long pelapsed_time,
      ComponentManager& pcomponent_manager,
      EntityManager& pentity_manager) noexcept;
  UpdateContext(UpdateContext const&) noexcept = delete;
  UpdateContext(UpdateContext&&) noexcept = delete;
  ~UpdateContext() noexcept;

  UpdateContext& operator=(UpdateContext const&) noexcept = delete;
  UpdateContext& operator=(UpdateContext&&) noexcept = delete;

  void removeEntity(Entity entity) noexcept;

  long elapsed_time;
  ComponentManager& component_manager;
  EntityManager& entity_manager;

private:
  mutable std::mutex mutex;
  std::queue<Entity> entity_delete_queue;
};
} // namespace redoom::ecs
