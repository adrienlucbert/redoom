#include <behaviours/extern.hh>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>

#include <redoom/Application.hh>
#include <redoom/ecs/components/CameraComponent.hh>
#include <redoom/events/Key.hh>

using redoom::ecs::Behaviour;
using redoom::ecs::Context;
using redoom::ecs::Entity;
using redoom::ecs::UpdateContext;
using redoom::ecs::components::CameraComponent;
using redoom::events::Key;
using redoom::events::MouseMoveEvent;
using redoom::events::ScrollEvent;
using redoom::events::WindowResizeEvent;

struct CameraBehaviour : public Behaviour {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"CameraBehaviour"};
    return type;
  }

  void onInit(Entity entity, Context& context) noexcept override
  {
    auto opt = context.component_manager.get<CameraComponent>(entity);
    if (!opt)
      assert("No camera component found" == nullptr);
    this->component = std::addressof(*opt);
  }

  void onUpdate(Entity /*entity*/, UpdateContext& context) noexcept override
  {
    auto camera_pos = this->component->camera.getPosition();
    auto const camera_speed = this->component->camera.getSpeed()
                            * static_cast<float>(context.elapsed_time * 1000);
    auto const camera_front = this->component->camera.getFront();
    auto const camera_up = this->component->camera.getUp();

    if (isKeyPressed(Key::W))
      camera_pos += camera_speed * camera_front;
    if (isKeyPressed(Key::S))
      camera_pos -= camera_speed * camera_front;
    if (isKeyPressed(Key::A))
      camera_pos -=
          glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    if (isKeyPressed(Key::D))
      camera_pos +=
          glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    this->component->camera.setPosition(camera_pos);
  }

  void onWindowResize(
      Entity /*entity*/, WindowResizeEvent& event) noexcept override
  {
    this->component->camera.setViewportSize(event.width, event.height);
  }

  void onMouseMove(Entity /*entity*/, MouseMoveEvent& event) noexcept override
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

  void onScroll(Entity /*entity*/, ScrollEvent& event) noexcept override
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
  CameraComponent* component{nullptr};
  bool mouse_never_moved{true};
  float last_mouse_x{0.0};
  float last_mouse_y{0.0};
};

std::unique_ptr<Behaviour> make() noexcept
{
  return std::make_unique<CameraBehaviour>();
}
