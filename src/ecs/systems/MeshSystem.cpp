#include <redoom/ecs/systems/MeshSystem.hh>

#include <redoom/ecs/components/MaterialComponent.hh>
#include <redoom/ecs/components/MeshComponent.hh>
#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/renderer/Renderer.hh>

namespace redoom::ecs::systems
{
void MeshSystem::update(UpdateContext& context) noexcept
{
  auto default_shader_opt = graphics::ShaderLibrary::getShader("lit");
  if (!default_shader_opt)
    assert("Undefined shader" == nullptr);
  auto& default_shader = *default_shader_opt;
  context.getComponentManager().apply<components::MeshComponent>(
      [&](auto entity, components::MeshComponent& component) {
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
        auto material_opt =
            context.getComponentManager().get<components::MaterialComponent>(
                entity);
        if (material_opt) {
          auto shader_opt = graphics::ShaderLibrary::getShader(
              (*material_opt).material.shader);
          if (!shader_opt)
            assert("Undefined shader" == nullptr);
          (*material_opt).material.apply(*shader_opt);
          renderer::Renderer::draw(*shader_opt, *component.mesh_, model);
        } else {
          renderer::Renderer::draw(default_shader, *component.mesh_, model);
        }
      });
}
} // namespace redoom::ecs::systems
