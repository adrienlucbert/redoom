#include <redoom/ecs/systems/LightSystem.hh>

#include <redoom/ecs/components/AmbientLightComponent.hh>
#include <redoom/ecs/components/DirectionalLightComponent.hh>
#include <redoom/renderer/Renderer.hh>

using redoom::ecs::components::AmbientLightComponent;
using redoom::ecs::components::DirectionalLightComponent;
using redoom::renderer::Renderer;

namespace redoom::ecs::systems
{
void LightSystem::update(UpdateContext& context) noexcept
{
  auto ambient_opt =
      context.getComponentManager().getOne<AmbientLightComponent>();
  if (ambient_opt.has_value()) {
    auto const& ambient = *ambient_opt;
    Renderer::setGlobalUniform("AmbientLightColor",
        graphics::uniforms::Vector<3>{.value = ambient.color_});
  }
  auto dir_opt =
      context.getComponentManager().getOne<DirectionalLightComponent>();
  if (dir_opt.has_value()) {
    auto const& dir = *dir_opt;
    Renderer::setGlobalUniform("DirectionalLightDirection",
        graphics::uniforms::Vector<3>{.value = dir.direction_});
    Renderer::setGlobalUniform("DirectionalLightColor",
        graphics::uniforms::Vector<3>{.value = dir.color_});
  }
}
} // namespace redoom::ecs::systems
