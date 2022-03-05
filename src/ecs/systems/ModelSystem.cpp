#include <redoom/ecs/systems/ModelSystem.hh>

#include <redoom/ecs/components/ModelComponent.hh>
#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/renderer/Renderer.hh>

namespace redoom::ecs::systems
{
void ModelSystem::update(UpdateContext& context) noexcept
{
  auto shader_opt = graphics::ShaderLibrary::getShader("default");
  if (!shader_opt)
    assert("Undefined shader" == nullptr);
  auto& shader = *shader_opt;
  context.getComponentManager().apply<components::ModelComponent>(
      [&](auto entity, components::ModelComponent& component) {
        auto model = glm::mat4(1.0f);
        auto transform_opt =
            context.getComponentManager().get<components::TransformComponent>(
                entity);
        if (transform_opt) {
          auto& transform = *transform_opt;
          model = glm::translate(model, transform.getPosition());
          model = glm::scale(model, transform.getScale());
          model =
              glm::rotate(model, transform.getAngle(), transform.getRotation());
        }
        renderer::Renderer::draw(shader, component.model_, model);
      });
}
} // namespace redoom::ecs::systems
