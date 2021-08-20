#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>

#include <redoom/Application.hh>
#include <redoom/ecs/Behaviour.hh>
#include <redoom/ecs/components/CameraComponent.hh>

namespace redoom::ecs::behaviours
{
struct CameraBehaviour : public redoom::ecs::Behaviour {
  void onInit(Entity entity, Context& context) noexcept override
  {
    auto exp =
        context.component_manager.get<components::CameraComponent>(entity);
    if (!exp)
      assert("No camera component found" == nullptr);
    this->component = std::addressof(*exp);
  }

  void onWindowResize(
      Entity /*entity*/, events::WindowResizeEvent& /*event*/) noexcept override
  {
    std::cout << "Resize camera" << '\n';
  }

  void onKey(Entity /*entity*/, events::KeyEvent& /*event*/) noexcept override
  {
    std::cout << "Move camera position" << '\n';
  }

  void onMouseMove(
      Entity /*entity*/, events::MouseMoveEvent& /*event*/) noexcept override
  {
    std::cout << "Move camera view" << '\n';
  }

  void onScroll(
      Entity /*entity*/, events::ScrollEvent& /*event*/) noexcept override
  {
    std::cout << "Change camera fov" << '\n';
  }

private:
  components::CameraComponent* component;
};
} // namespace redoom::ecs::behaviours
