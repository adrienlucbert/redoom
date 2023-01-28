#include <redoom/ecs/systems/RenderSystem.hh>

#include <Utils/Concepts.hpp>
#include <redoom/ecs/components/MaterialComponent.hh>
#include <redoom/ecs/components/MeshComponent.hh>
#include <redoom/ecs/components/ModelComponent.hh>
#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/renderer/Renderer.hh>

namespace redoom::ecs::systems
{
namespace detail
{
template <concepts::Drawable T>
static void renderDrawable(
    UpdateContext& context, Entity entity, T const& drawable)
{
  auto model = glm::mat4(1.0f);
  auto transform_opt =
      context.getComponentManager().get<components::TransformComponent>(entity);
  if (transform_opt) {
    auto& transform = *transform_opt;
    model = glm::translate(model, transform.getPosition());
    model = glm::rotate(model, transform.getAngle(), transform.getRotation());
    model = glm::scale(model, transform.getScale());
  }
  renderer::Renderer::get().setGlobalUniform("EntityId",
      graphics::uniforms::Uint{.value = static_cast<GLuint>(entity)});
  auto material_opt =
      context.getComponentManager().get<components::MaterialComponent>(entity);
  if (material_opt) {
    (*material_opt).material.use();
  } else {
    renderer::Renderer::get().getRenderPipeline().getDefaultMaterial().use();
  }
  renderer::Renderer::get().draw(drawable, model);
}
} // namespace detail

void RenderSystem::beforeUpdate(UpdateContext& /*context*/) noexcept
{
  renderer::Renderer::get().beginRendering();
}

void RenderSystem::update(UpdateContext& context) noexcept
{
  context.getComponentManager().apply<components::MeshComponent>(
      [&](auto entity, components::MeshComponent& component) {
        detail::renderDrawable(context, entity, *component.mesh_);
      });
  context.getComponentManager().apply<components::ModelComponent>(
      [&](auto entity, components::ModelComponent& component) {
        detail::renderDrawable(context, entity, component.model_);
      });
}

void RenderSystem::afterUpdate(UpdateContext& /*context*/) noexcept
{
  renderer::Renderer::get().endRendering();
}
} // namespace redoom::ecs::systems
