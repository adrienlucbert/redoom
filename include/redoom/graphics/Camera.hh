#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <yaml-cpp/yaml.h>

namespace redoom::graphics
{
class Camera
{
public:
  enum class ProjectionType { Perspective, Orthographic };

  explicit Camera(glm::vec3 position = glm::vec3{0.0f, 0.0f, 0.0f}) noexcept;
  Camera(Camera const& b) noexcept = default;
  Camera(Camera&& b) noexcept = default;
  ~Camera() noexcept = default;

  Camera& operator=(Camera const& rhs) noexcept = default;
  Camera& operator=(Camera&& rhs) noexcept = default;

  [[nodiscard]] glm::vec3 const& getPosition() const noexcept;
  void setPosition(glm::vec3 position) noexcept;
  [[nodiscard]] glm::vec3 const& getFront() const noexcept;
  void setFront(glm::vec3 front) noexcept;
  [[nodiscard]] glm::vec3 const& getUp() const noexcept;
  void setUp(glm::vec3 up) noexcept;
  [[nodiscard]] glm::vec3 const& getRight() const noexcept;
  void setRight(glm::vec3 right) noexcept;
  [[nodiscard]] float getYaw() const noexcept;
  void setYaw(float yaw) noexcept;
  [[nodiscard]] float getPitch() const noexcept;
  void setPitch(float pitch) noexcept;
  [[nodiscard]] float getSpeed() const noexcept;
  void setSpeed(float speed) noexcept;
  [[nodiscard]] float getSensitivity() const noexcept;
  void setSensitivity(float sensitivity) noexcept;
  [[nodiscard]] float getFov() const noexcept;
  void setFov(float fov) noexcept;
  void setViewportSize(int width, int height) noexcept;

  void translate(glm::vec3 translation) noexcept;

  [[nodiscard]] glm::mat4 const& getView() const noexcept;

  [[nodiscard]] glm::mat4 const& getProjection() const noexcept;

private:
  void updateProjection() noexcept;
  void updateView() noexcept;

  glm::vec3 position_;
  mutable glm::vec3 front_{0.0f, 0.0f, -1.0f};
  mutable glm::vec3 up_{0.0f, 1.0f, 0.0f};
  mutable glm::vec3 right_{};
  glm::vec3 world_up_{0.0f, 1.0f, 0.0f};
  float yaw_{-90.0f};
  float pitch_{0.0f};
  float speed_{0.01f};
  float sensitivity_{0.3f};
  float fov_{glm::radians(45.0f)};
  float aspect_ratio_{0.0f};
  ProjectionType projection_type_{ProjectionType::Perspective};
  glm::mat4 view_{1.0f};
  glm::mat4 projection_{1.0f};
};
} // namespace redoom::graphics

namespace YAML
{
YAML::Emitter& operator<<(
    YAML::Emitter& out, const redoom::graphics::Camera& c);

template <>
struct convert<redoom::graphics::Camera> {
  static Node encode(const redoom::graphics::Camera& rhs);
  static bool decode(const Node& node, redoom::graphics::Camera& rhs);
};
} // namespace YAML
