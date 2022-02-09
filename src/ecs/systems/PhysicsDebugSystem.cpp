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
          for (auto const& fixture : component.body->getFixtures()) {
            auto model = glm::mat4(1.0f);
            if (transform_opt) {
              auto& transform = *transform_opt;
              model = glm::translate(model,
                  transform.position
                      + fixture.getLocalPosition() * transform.scale);
              model = glm::scale(model, transform.scale);
              model = glm::rotate(model, transform.angle, transform.rotation);
            }
            renderer::Renderer::draw(shader, fixture.getShape(), model);
          }
          auto aabb = component.body->getGlobalAABB();
          if (aabb.has_value()) {
            auto cuboid = graphics::mesh::Cuboid{
                aabb->upper_bounds.x - aabb->lower_bounds.x,
                aabb->upper_bounds.y - aabb->lower_bounds.y,
                aabb->upper_bounds.z - aabb->lower_bounds.z,
                {0.0f, 1.0f, 0.0f},
                {},
                GL_LINE_STRIP};
            auto offset = aabb->getCenter();
            /* The cuboid being centered on 0, in order to center the rendered
             * hitbox on the AABB coordinates, we need to translate the
             * rendering model by the AABB center position.
             */
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, offset);
            if (transform_opt) {
              auto& transform = *transform_opt;
              model = glm::rotate(model, transform.angle, transform.rotation);
            }
            renderer::Renderer::draw(shader, cuboid, model);
          }
        });
  }
}
} // namespace redoom::ecs::systems
