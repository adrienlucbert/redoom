#pragma once

#include <mutex>
#include <queue>

#include <redoom/ecs/ComponentManager.hh>
#include <redoom/ecs/EntityManager.hh>
#include <redoom/renderer/Window.hh>

namespace redoom::ecs
{
struct UpdateContext {
  UpdateContext(double pelapsed_time,
      ComponentManager& pcomponent_manager,
      EntityManager& pentity_manager,
      renderer::Window& pwindow) noexcept;
  UpdateContext(UpdateContext const&) noexcept = delete;
  UpdateContext(UpdateContext&&) noexcept = delete;
  ~UpdateContext() noexcept;

  UpdateContext& operator=(UpdateContext const&) noexcept = delete;
  UpdateContext& operator=(UpdateContext&&) noexcept = delete;

  void removeEntity(Entity entity) noexcept;

  double elapsed_time;
  ComponentManager& component_manager;
  EntityManager& entity_manager;
  renderer::Window& window;

private:
  mutable std::mutex mutex;
  std::queue<Entity> entity_delete_queue;
};
} // namespace redoom::ecs
