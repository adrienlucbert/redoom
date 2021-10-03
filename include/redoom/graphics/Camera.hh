#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <yaml-cpp/yaml.h>

namespace redoom::graphics
{
class Camera
{
public:
  enum class ProjectionType { Perspective, Orthographic };

  explicit Camera(glm::vec3 pposition = glm::vec3{0.0f, 0.0f, 0.0f}) noexcept;
  Camera(Camera const& b) noexcept = default;
  Camera(Camera&& b) noexcept = default;
  ~Camera() noexcept = default;

  Camera& operator=(Camera const& rhs) noexcept = default;
  Camera& operator=(Camera&& rhs) noexcept = default;

  [[nodiscard]] glm::vec3 const& getPosition() const noexcept;
  void setPosition(glm::vec3 pposition) noexcept;
  [[nodiscard]] glm::vec3 const& getFront() const noexcept;
  void setFront(glm::vec3 pfront) noexcept;
  [[nodiscard]] glm::vec3 const& getUp() const noexcept;
  void setUp(glm::vec3 pup) noexcept;
  [[nodiscard]] glm::vec3 const& getRight() const noexcept;
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
  void setViewportSize(int width, int height) noexcept;

  void translate(glm::vec3 translation) noexcept;

  [[nodiscard]] glm::mat4 const& getView() const noexcept;

  [[nodiscard]] glm::mat4 const& getProjection() const noexcept;

private:
  void updateProjection() noexcept;
  void updateView() noexcept;

  glm::vec3 position;
  mutable glm::vec3 front{0.0f, 0.0f, -1.0f};
  mutable glm::vec3 up{0.0f, 1.0f, 0.0f};
  mutable glm::vec3 right{};
  glm::vec3 world_up{0.0f, 1.0f, 0.0f};
  float yaw{-90.0f};
  float pitch{0.0f};
  float speed{0.01f};
  float sensitivity{0.3f};
  float fov{glm::radians(45.0f)};
  float aspect_ratio{0.0f};
  ProjectionType projection_type{ProjectionType::Perspective};
  glm::mat4 view{1.0f};
  glm::mat4 projection{1.0f};
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
