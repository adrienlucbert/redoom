#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>

#include <redoom/Application.hh>
#include <redoom/ecs/Behaviour.hh>
#include <redoom/ecs/components/CameraComponent.hh>
#include <redoom/events/Key.hh>

namespace redoom::ecs::behaviours
{
struct CameraBehaviour : public redoom::ecs::Behaviour {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"CameraBehaviour"};
    return type;
  }

  void onInit(Entity entity, Context& context) noexcept override
  {
    auto exp =
        context.component_manager.get<components::CameraComponent>(entity);
    if (!exp)
      assert("No camera component found" == nullptr);
    this->component = std::addressof(*exp);
  }

  void onUpdate(Entity /*entity*/, UpdateContext& context) noexcept override
  {
    auto camera_pos = this->component->camera.getPosition();
    auto const camera_speed = this->component->camera.getSpeed()
                            * static_cast<float>(context.elapsed_time * 1000);
    auto const camera_front = this->component->camera.getFront();
    auto const camera_up = this->component->camera.getUp();

    if (events::isKeyPressed(events::Key::W))
      camera_pos += camera_speed * camera_front;
    if (events::isKeyPressed(events::Key::S))
      camera_pos -= camera_speed * camera_front;
    if (events::isKeyPressed(events::Key::A))
      camera_pos -=
          glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    if (events::isKeyPressed(events::Key::D))
      camera_pos +=
          glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    this->component->camera.setPosition(camera_pos);
  }

  void onWindowResize(
      Entity /*entity*/, events::WindowResizeEvent& event) noexcept override
  {
    this->component->camera.setViewportSize(event.width, event.height);
  }

  void onMouseMove(
      Entity /*entity*/, events::MouseMoveEvent& event) noexcept override
  {
    if (this->mouse_never_moved) {
      this->last_mouse_x = static_cast<float>(event.x_pos);
      this->last_mouse_y = static_cast<float>(event.y_pos);
      this->mouse_never_moved = false;
    }

    auto x_offset = static_cast<float>(event.x_pos) - this->last_mouse_x;
    auto y_offset = this->last_mouse_y - static_cast<float>(event.y_pos);
    this->last_mouse_x = static_cast<float>(event.x_pos);
    this->last_mouse_y = static_cast<float>(event.y_pos);

    auto const sensitivity = this->component->camera.getSensitivity();
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    auto yaw = this->component->camera.getYaw();
    auto pitch = this->component->camera.getPitch();
    yaw += x_offset;
    pitch += y_offset;
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;
    this->component->camera.setYaw(yaw);
    this->component->camera.setPitch(pitch);

    auto direction =
        glm::vec3{std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
            std::sin(glm::radians(pitch)),
            std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch))};
    this->component->camera.setFront(glm::normalize(direction));
  }

  void onScroll(Entity /*entity*/, events::ScrollEvent& event) noexcept override
  {
    auto fov = glm::degrees(this->component->camera.getFov());
    fov -= static_cast<float>(event.y_offset);
    if (fov < 1.0f)
      fov = 1.0f;
    if (fov > 45.0f)
      fov = 45.0f;
    this->component->camera.setFov(glm::radians(fov));
  }

private:
  components::CameraComponent* component;
  bool mouse_never_moved{true};
  float last_mouse_x{0.0};
  float last_mouse_y{0.0};
};
} // namespace redoom::ecs::behaviours
