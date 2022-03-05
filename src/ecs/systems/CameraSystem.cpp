#include <redoom/ecs/systems/CameraSystem.hh>

#include <redoom/ecs/components/CameraComponent.hh>
#include <redoom/renderer/Renderer.hh>

using redoom::ecs::components::CameraComponent;

namespace redoom::ecs::systems
{
void CameraSystem::update(UpdateContext& context) noexcept
{
  auto count = 0u;
  context.getComponentManager().apply<components::CameraComponent>(
      [&count](auto /*entity*/, auto const& component) {
        ++count;
        assert(count <= 1 && "More than one camera is not allowed");
        renderer::Renderer::setViewMatrix(component.camera_.getView());
        renderer::Renderer::setProjectionMatrix(
            component.camera_.getProjection());
      });
}
} // namespace redoom::ecs::systems
