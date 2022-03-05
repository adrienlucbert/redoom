#include <behaviours/extern.hh>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
    auto opt = context.getComponentManager().get<CameraComponent>(entity);
    if (!opt)
      assert("No camera component found" == nullptr);
    this->component_ = std::addressof(*opt);
    this->initial_camera_speed_ = this->component_->camera_.getSpeed();
  }

  void onUpdate(Entity /*entity*/, UpdateContext& context) noexcept override
  {
    auto camera_pos = this->component_->camera_.getPosition();
    auto const camera_speed =
        this->component_->camera_.getSpeed()
        * static_cast<float>(context.getElapsedTime() * 1000);
    auto const camera_front = this->component_->camera_.getFront();
    auto const camera_up = this->component_->camera_.getUp();

    if (isKeyPressed(Key::LEFT_SHIFT) || isKeyPressed(Key::RIGHT_SHIFT))
      this->component_->camera_.setSpeed(this->initial_camera_speed_ * 10);
    else
      this->component_->camera_.setSpeed(this->initial_camera_speed_);
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
    this->component_->camera_.setPosition(camera_pos);
  }

  void onWindowResize(
      Entity /*entity*/, WindowResizeEvent& event) noexcept override
  {
    this->component_->camera_.setViewportSize(event.width, event.height);
  }

  void onMouseMove(Entity /*entity*/, MouseMoveEvent& event) noexcept override
  {
    if (this->mouse_never_moved_) {
      this->last_mouse_x_ = static_cast<float>(event.x_pos);
      this->last_mouse_y_ = static_cast<float>(event.y_pos);
      this->mouse_never_moved_ = false;
    }

    auto x_offset = static_cast<float>(event.x_pos) - this->last_mouse_x_;
    auto y_offset = this->last_mouse_y_ - static_cast<float>(event.y_pos);
    this->last_mouse_x_ = static_cast<float>(event.x_pos);
    this->last_mouse_y_ = static_cast<float>(event.y_pos);

    auto const sensitivity = this->component_->camera_.getSensitivity();
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    auto yaw = this->component_->camera_.getYaw();
    auto pitch = this->component_->camera_.getPitch();
    yaw += x_offset;
    pitch += y_offset;
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;
    this->component_->camera_.setYaw(yaw);
    this->component_->camera_.setPitch(pitch);

    auto direction =
        glm::vec3{std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
            std::sin(glm::radians(pitch)),
            std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch))};
    this->component_->camera_.setFront(glm::normalize(direction));
  }

  void onScroll(Entity /*entity*/, ScrollEvent& event) noexcept override
  {
    auto fov = glm::degrees(this->component_->camera_.getFov());
    fov -= static_cast<float>(event.y_offset);
    if (fov < 1.0f)
      fov = 1.0f;
    if (fov > 45.0f)
      fov = 45.0f;
    this->component_->camera_.setFov(glm::radians(fov));
  }

private:
  CameraComponent* component_{nullptr};
  float initial_camera_speed_{0.0f};
  bool mouse_never_moved_{true};
  float last_mouse_x_{0.0f};
  float last_mouse_y_{0.0f};
};

std::unique_ptr<Behaviour> make() noexcept
{
  return std::make_unique<CameraBehaviour>();
}
