#pragma once

#include <redoom/ecs/Context.hh>
#include <redoom/renderer/Window.hh>

namespace redoom::ecs
{
struct UpdateContext : public Context {
  UpdateContext(ComponentManager& pcomponent_manager,
      EntityManager& pentity_manager,
      double pelapsed_time,
      renderer::Window& pwindow) noexcept;
  UpdateContext(UpdateContext const&) noexcept = delete;
  UpdateContext(UpdateContext&&) noexcept = delete;
  ~UpdateContext() noexcept = default;

  UpdateContext& operator=(UpdateContext const&) noexcept = delete;
  UpdateContext& operator=(UpdateContext&&) noexcept = delete;

  double elapsed_time;
  renderer::Window& window;
};
} // namespace redoom::ecs
