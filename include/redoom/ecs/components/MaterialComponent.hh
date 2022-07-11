#pragma once

#include <memory>

#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/graphics/Material.hh>

namespace redoom::ecs::components
{
struct MaterialComponent : public Component<MaterialComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"MaterialComponent"};
    return type;
  }

  explicit MaterialComponent(graphics::Material pmaterial)
    : material{std::move(pmaterial)}
  {
  }

  graphics::Material material;
};
} // namespace redoom::ecs::components
