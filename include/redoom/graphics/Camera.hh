#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace redoom::graphics
{
class Camera
{
public:
  explicit Camera(glm::vec3 pposition = glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3 pup = glm::vec3(0.0f, 1.0f, 0.0f),
      float pyaw = -90.f,
      float ppitch = 0.f) noexcept;
  Camera(Camera const& b) noexcept = default;
  Camera(Camera&& b) noexcept = default;
  ~Camera() noexcept = default;

  Camera& operator=(Camera const& rhs) noexcept = default;
  Camera& operator=(Camera&& rhs) noexcept = default;

  [[nodiscard]] glm::vec3 getPosition() const noexcept;
  void setPosition(glm::vec3 pposition) noexcept;
  [[nodiscard]] glm::vec3 getFront() const noexcept;
  void setFront(glm::vec3 pfront) noexcept;
  [[nodiscard]] glm::vec3 getUp() const noexcept;
  void setUp(glm::vec3 pup) noexcept;
  [[nodiscard]] glm::vec3 getRight() const noexcept;
  void setRight(glm::vec3 pright) noexcept;
  [[nodiscard]] float getYaw() const noexcept;
  void setYaw(float pyaw) noexcept;
  [[nodiscard]] float getPitch() const noexcept;
  void setPitch(float ppitch) noexcept;
  [[nodiscard]] float getSpeed() const noexcept;
  void setSpeed(float pspeed) noexcept;
  [[nodiscard]] float getSensitivity() const noexcept;
  void setSensitivity(float psensitivity) noexcept;
  [[nodiscard]] float getFov() const noexcept;
  void setFov(float pfov) noexcept;

  void translate(glm::vec3 translation) noexcept;

  [[nodiscard]] glm::mat4 getView() const noexcept;

private:
  void update() const noexcept;

  glm::vec3 position;
  mutable glm::vec3 front;
  mutable glm::vec3 up;
  mutable glm::vec3 right;
  glm::vec3 world_up;
  float yaw;
  float pitch;
  float speed;
  float sensitivity;
  float fov;
  mutable bool needs_update{true};
};
} // namespace redoom::graphics
