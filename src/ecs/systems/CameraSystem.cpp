#include <redoom/ecs/systems/CameraSystem.hh>

#include <redoom/ecs/components/CameraComponent.hh>
#include <redoom/renderer/Renderer.hh>

using redoom::ecs::components::CameraComponent;

namespace redoom::ecs::systems
{
void CameraSystem::update(UpdateContext& context) noexcept
{
  auto count = 0u;
  context.component_manager.apply<components::CameraComponent>(
      [&count](auto /*entity*/, auto const& component) {
        if (++count > 1)
          assert("More than one camera is not allowed" == nullptr);
        renderer::Renderer::setViewProjectionMatrix(component.camera.getView());
      });
}
} // namespace redoom::ecs::systems
