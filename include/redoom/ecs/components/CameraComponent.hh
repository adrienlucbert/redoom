#pragma once

#include <redoom/ecs/Component.hh>
#include <redoom/graphics/Camera.hh>

namespace redoom::ecs::components
{
struct CameraComponent : public Component<CameraComponent> {
  explicit CameraComponent(
      graphics::Camera pcamera = graphics::Camera{}) noexcept
    : camera{pcamera}
  {
  }
  graphics::Camera camera;
};
} // namespace redoom::ecs::components
