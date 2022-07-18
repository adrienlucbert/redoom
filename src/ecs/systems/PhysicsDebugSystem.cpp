#include <redoom/ecs/systems/PhysicsDebugSystem.hh>

#include <redoom/Application.hh>
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
    auto program_opt = graphics::ShaderLibrary::getShader("unlit");
    if (!program_opt)
      assert("Undefined program" == nullptr);
    auto& program = *program_opt;
    renderer::Renderer::get().useProgram(program);

    auto old_topology = renderer::Renderer::get().getTopology();
    renderer::Renderer::get().setTopology(GL_LINE_STRIP);
    context.getComponentManager().apply<components::BodyComponent>(
        [&](auto entity, components::BodyComponent& component) {
          auto transform_opt =
              context.getComponentManager().get<components::TransformComponent>(
                  entity);
          for (auto const& fixture : component.body_->getFixtures()) {
            auto model = glm::mat4(1.0f);
            if (transform_opt) {
              auto& transform = *transform_opt;
              model = glm::translate(model,
                  transform.getPosition()
                      + fixture.getLocalPosition() * transform.getScale());
              model = glm::scale(model, transform.getScale());
              model = glm::rotate(
                  model, transform.getAngle(), transform.getRotation());
            }
            renderer::Renderer::get().draw(*fixture.getShape(), model);
          }
          auto aabb = component.body_->getGlobalAABB();
          if (aabb.has_value()) {
            auto cuboid = graphics::mesh::Cuboid{
                aabb->upper_bounds.x - aabb->lower_bounds.x,
                aabb->upper_bounds.y - aabb->lower_bounds.y,
                aabb->upper_bounds.z - aabb->lower_bounds.z};
            auto offset = aabb->getCenter();
            /* The cuboid being centered on 0, in order to center the rendered
             * hitbox on the AABB coordinates, we need to translate the
             * rendering model by the AABB center position.
             */
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, offset);
            renderer::Renderer::get().draw(cuboid, model);
          }
          Application::get().getCurrentScene().getWorld().debugDraw();
        });
    renderer::Renderer::get().setTopology(old_topology);
  }
}
} // namespace redoom::ecs::systems
