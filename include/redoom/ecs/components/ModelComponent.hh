#pragma once

#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/ecs/components/BodyComponent.hh>
#include <redoom/graphics/Model.hh>

namespace redoom::ecs::components
{
struct ModelComponent : public Component<ModelComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"ModelComponent"};
    return type;
  }

  explicit ModelComponent(graphics::Model model)
    : model_{std::move(model)}
  {
  }

  graphics::Model model_;
};
} // namespace redoom::ecs::components
