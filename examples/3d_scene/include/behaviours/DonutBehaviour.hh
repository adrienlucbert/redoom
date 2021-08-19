#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <redoom/ecs/Behaviour.hh>

#include <iostream>

#include <fmt/format.h>
#include <glm/gtc/type_ptr.hpp>

#include <redoom/ecs/components/TransformComponent.hh>

struct DonutBehaviour : public redoom::ecs::Behaviour {
  void onUpdate(redoom::ecs::Entity /*entity*/,
      redoom::ecs::UpdateContext& /*context*/) noexcept override
  {
    // auto transform_opt =
    //     context.component_manager
    //         .get<redoom::ecs::components::TransformComponent>(entity);
    // auto& transform = *transform_opt;
    // transform.angle += static_cast<float>(context.elapsed_time * 5);
  }

  void onWindowResize(redoom::ecs::Entity /*entity*/,
      redoom::events::WindowResizeEvent& event) noexcept override
  {
    std::cout << fmt::format(
        "Window resize: {} x {}", event.width, event.height)
              << '\n';
  }

  void onWindowClose(redoom::ecs::Entity /*entity*/,
      redoom::events::WindowCloseEvent& /*event*/) noexcept override
  {
    std::cout << "Window close" << '\n';
  }

  void onKey(redoom::ecs::Entity /*entity*/,
      redoom::events::KeyEvent& event) noexcept override
  {
    auto const* action = [&event]() {
      if (event.action == redoom::events::Action::PRESS)
        return "pressed";
      else if (event.action == redoom::events::Action::REPEAT)
        return "repeated";
      else if (event.action == redoom::events::Action::RELEASE)
        return "released";
      else
        return "<unknown action>";
    }();
    std::cout << fmt::format("Key {}: {}", action, event.key) << '\n';
  }

  void onChar(redoom::ecs::Entity /*entity*/,
      redoom::events::CharEvent& event) noexcept override
  {
    std::cout << fmt::format("Char: {}", static_cast<char>(event.keycode))
              << '\n';
  }

  void onMouseButton(redoom::ecs::Entity /*entity*/,
      redoom::events::MouseButtonEvent& event) noexcept override
  {
    auto const* action = [&event]() {
      if (event.action == redoom::events::Action::PRESS)
        return "pressed";
      else if (event.action == redoom::events::Action::REPEAT)
        return "repeated";
      else if (event.action == redoom::events::Action::RELEASE)
        return "released";
      else
        return "<unknown action>";
    }();
    std::cout << fmt::format("Mouse button {}: {}", action, event.button)
              << '\n';
  }

  void onMouseMove(redoom::ecs::Entity /*entity*/,
      redoom::events::MouseMoveEvent& event) noexcept override
  {
    std::cout << fmt::format(
        "Mouse moved: x: {} y: {}", event.x_pos, event.y_pos)
              << '\n';
  }

  void onScroll(redoom::ecs::Entity /*entity*/,
      redoom::events::ScrollEvent& event) noexcept override
  {
    std::cout << fmt::format(
        "Mouse scrolled: x: {} y: {}", event.x_offset, event.y_offset)
              << '\n';
  }
};
