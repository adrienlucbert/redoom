#include <redoom/ecs/UpdateContext.hh>

namespace redoom::ecs
{
UpdateContext::UpdateContext(ComponentManager& component_manager,
    EntityManager& entity_manager,
    double elapsed_time,
    renderer::Window& window) noexcept
  : Context(component_manager, entity_manager)
  , elapsed_time_{elapsed_time}
  , window_{window}
{
}

double UpdateContext::getElapsedTime() const noexcept
{
  return this->elapsed_time_;
}

renderer::Window& UpdateContext::getWindow() const noexcept
{
  return this->window_;
}
} // namespace redoom::ecs
