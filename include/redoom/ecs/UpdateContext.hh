#pragma once

#include <redoom/ecs/Context.hh>
#include <redoom/renderer/Window.hh>

namespace redoom::ecs
{
class UpdateContext : public Context
{
public:
  UpdateContext(ComponentManager& component_manager,
      EntityManager& entity_manager,
      double elapsed_time,
      renderer::Window& window) noexcept;
  UpdateContext(UpdateContext const&) noexcept = delete;
  UpdateContext(UpdateContext&&) noexcept = delete;
  ~UpdateContext() noexcept = default;

  UpdateContext& operator=(UpdateContext const&) noexcept = delete;
  UpdateContext& operator=(UpdateContext&&) noexcept = delete;

  [[nodiscard]] double getElapsedTime() const noexcept;
  [[nodiscard]] renderer::Window& getWindow() const noexcept;

private:
  double elapsed_time_;
  renderer::Window& window_;
};
} // namespace redoom::ecs
