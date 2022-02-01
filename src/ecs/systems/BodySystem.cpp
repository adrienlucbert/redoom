#include <redoom/ecs/systems/BodySystem.hh>

#include <redoom/ecs/components/BodyComponent.hh>
#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/renderer/Renderer.hh>

namespace redoom::ecs::systems
{
void BodySystem::update(UpdateContext& context) noexcept
{
  auto shader_opt = graphics::ShaderLibrary::getShader("default");
  if (!shader_opt)
    assert("Undefined shader" == nullptr);
  auto& shader = *shader_opt;

  context.component_manager.apply<components::BodyComponent>(
      [&](auto entity, components::BodyComponent& component) {
        auto model = glm::mat4(1.0f);
        auto transform_opt =
            context.component_manager.get<components::TransformComponent>(
                entity);
        if (transform_opt) {
          auto& transform = *transform_opt;
          model = glm::translate(model, transform.position);
          model = glm::scale(model, transform.scale);
          model = glm::rotate(model, transform.angle, transform.rotation);
        }
        renderer::Renderer::draw(shader, component.body, model);
      });
}
} // namespace redoom::ecs::systems
