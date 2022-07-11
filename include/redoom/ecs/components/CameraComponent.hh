#pragma once

#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/graphics/Camera.hh>

namespace redoom::ecs::components
{
struct CameraComponent : public Component<CameraComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"CameraComponent"};
    return type;
  }

  explicit CameraComponent(
      graphics::Camera camera = graphics::Camera{}) noexcept
    : camera_{camera}
  {
  }

  graphics::Camera camera_;
};
} // namespace redoom::ecs::components
