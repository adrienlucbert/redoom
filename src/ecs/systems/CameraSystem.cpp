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
        renderer::Renderer::setGlobalUniform("CameraPosition",
            graphics::uniforms::Vector<3>{
                .value = component.camera_.getPosition()});
        renderer::Renderer::setGlobalUniform("view",
            graphics::uniforms::Matrix<4, 4>{
                .value = component.camera_.getView(), .transpose = GL_FALSE});
        renderer::Renderer::setGlobalUniform("projection",
            graphics::uniforms::Matrix<4, 4>{
                .value = component.camera_.getProjection(),
                .transpose = GL_FALSE});
      });
}
} // namespace redoom::ecs::systems
