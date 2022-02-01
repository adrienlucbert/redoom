#include <redoom/ecs/systems/PhysicsWorldSystem.hh>

#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/renderer/Renderer.hh>

namespace redoom::ecs::systems
{
void PhysicsWorldSystem::update(UpdateContext& context) noexcept
{
  // auto shader_opt = graphics::ShaderLibrary::getShader("default");
  // if (!shader_opt)
  //   assert("Undefined shader" == nullptr);
  // auto& shader = *shader_opt;
  // context.component_manager.apply<components::PhysicsWorldComponent>(
  //     [&](auto entity, components::PhysicsWorldComponent& component) {
  //       auto model = glm::mat4(1.0f);
  //       auto transform_opt =
  //           context.component_manager.get<components::TransformComponent>(
  //               entity);
  //       if (transform_opt) {
  //         auto& transform = *transform_opt;
  //         model = glm::translate(model, transform.position);
  //         model = glm::scale(model, transform.scale);
  //         model = glm::rotate(model, transform.angle, transform.rotation);
  //       }
  //       renderer::Renderer::draw(shader, component.model, model);
  //     });
}
} // namespace redoom::ecs::systems
