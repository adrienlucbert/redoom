#include <redoom/ecs/systems/PhysicsDebugSystem.hh>

#include <redoom/ecs/components/BodyComponent.hh>
#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/renderer/Renderer.hh>

namespace redoom::ecs::systems
{
void PhysicsDebugSystem::update(UpdateContext& context) noexcept
{
  // Debug mode: draw bodies fixtures
  if (Application::get().getCurrentScene().getWorld().getDebugDraw()) {
    auto shader_opt = graphics::ShaderLibrary::getShader("default");
    if (!shader_opt)
      assert("Undefined shader" == nullptr);
    auto& shader = *shader_opt;

    context.component_manager.apply<components::BodyComponent>(
        [&](auto entity, components::BodyComponent& component) {
          auto transform_opt =
              context.component_manager.get<components::TransformComponent>(
                  entity);
          for (auto const& fixture : component.body.get().getFixtures()) {
            auto model = glm::mat4(1.0f);
            if (transform_opt) {
              auto& transform = *transform_opt;
              model = glm::translate(model,
                  transform.position + fixture.getPosition() * transform.scale);
              model = glm::scale(model, transform.scale);
              model = glm::rotate(model, transform.angle, transform.rotation);
            }
            renderer::Renderer::draw(shader, fixture.getShape(), model);
          }
        });
  }
}
} // namespace redoom::ecs::systems
