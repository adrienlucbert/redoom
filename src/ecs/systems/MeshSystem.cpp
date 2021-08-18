#include <redoom/ecs/systems/MeshSystem.hh>

#include <redoom/ecs/components/MeshComponent.hh>
#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/renderer/Renderer.hh>

namespace redoom::ecs::systems
{
void MeshSystem::update(UpdateContext& context) noexcept
{
  auto shader_opt = graphics::ShaderLibrary::getShader("default");
  if (!shader_opt)
    assert("Undefined shader" == nullptr);
  auto& shader = *shader_opt;
  context.component_manager.apply<components::MeshComponent>(
      [&](auto entity, components::MeshComponent& component) {
        auto model = glm::mat4(1.0f);
        auto transform_opt =
            context.component_manager.get<components::TransformComponent>(
                entity);
        auto& transform = *transform_opt;
        if (transform_opt) {
          model = glm::translate(model, transform.position);
          model = glm::scale(model, transform.scale);
          model = glm::rotate(model, transform.angle, transform.rotation);
        }
        renderer::Renderer::draw(shader, *component.mesh, model);
      });
}
} // namespace redoom::ecs::systems
