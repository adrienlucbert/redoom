#pragma once

#include <redoom/ecs/Component.hh>
#include <redoom/graphics/Camera.hh>

namespace redoom::ecs::components
{
struct CameraComponent : public Component<CameraComponent> {
  graphics::Camera camera;
};
} // namespace redoom::ecs::components
