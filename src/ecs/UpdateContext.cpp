#include <redoom/ecs/UpdateContext.hh>

namespace redoom::ecs
{
UpdateContext::UpdateContext(ComponentManager& pcomponent_manager,
    EntityManager& pentity_manager,
    double pelapsed_time,
    renderer::Window& pwindow) noexcept
  : Context(pcomponent_manager, pentity_manager)
  , elapsed_time{pelapsed_time}
  , window{pwindow}
{
}
} // namespace redoom::ecs
