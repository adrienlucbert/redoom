#include <redoom/graphics/Camera.hh>

#include <redoom/serializer/common.hh>

namespace redoom::graphics
{
Camera::Camera(glm::vec3 position) noexcept
  : position_{position}
{
  this->updateView();
  this->updateProjection();
}

glm::vec3 const& Camera::getPosition() const noexcept
{
  return this->position_;
}

void Camera::setPosition(glm::vec3 position) noexcept
{
  this->position_ = position;
  this->updateView();
}

glm::vec3 const& Camera::getFront() const noexcept
{
  return this->front_;
}

void Camera::setFront(glm::vec3 front) noexcept
{
  this->front_ = front;
  this->updateView();
}

glm::vec3 const& Camera::getUp() const noexcept
{
  return this->up_;
}

void Camera::setUp(glm::vec3 up) noexcept
{
  this->up_ = up;
  this->updateView();
}

glm::vec3 const& Camera::getRight() const noexcept
{
  return this->right_;
}

void Camera::setRight(glm::vec3 right) noexcept
{
  this->right_ = right;
  this->updateView();
}

float Camera::getYaw() const noexcept
{
  return this->yaw_;
}

void Camera::setYaw(float yaw) noexcept
{
  this->yaw_ = yaw;
  this->updateView();
}

float Camera::getPitch() const noexcept
{
  return this->pitch_;
}

void Camera::setPitch(float pitch) noexcept
{
  this->pitch_ = pitch;
  this->updateView();
}

float Camera::getSpeed() const noexcept
{
  return this->speed_;
}

void Camera::setSpeed(float speed) noexcept
{
  this->speed_ = speed;
}

float Camera::getSensitivity() const noexcept
{
  return this->sensitivity_;
}

void Camera::setSensitivity(float sensitivity) noexcept
{
  this->sensitivity_ = sensitivity;
}

float Camera::getFov() const noexcept
{
  return this->fov_;
}

void Camera::setFov(float fov) noexcept
{
  this->fov_ = fov;
  this->updateProjection();
}

void Camera::setViewportSize(int width, int height) noexcept
{
  assert(width > 0 && height > 0);
  this->aspect_ratio_ = static_cast<float>(width) / static_cast<float>(height);
  this->updateProjection();
}

glm::mat4 const& Camera::getView() const noexcept
{
  return this->view_;
}

glm::mat4 const& Camera::getProjection() const noexcept
{
  return this->projection_;
}

void Camera::updateProjection() noexcept
{
  if (this->projection_type_ == ProjectionType::Perspective) {
    this->projection_ =
        glm::perspective(this->fov_, this->aspect_ratio_, 0.001f, 1000.0f);
  } else // TODO(alucbert): add support for orthographic projection
    assert("Orthographic projection is not yet supported" == nullptr);
}

void Camera::updateView() noexcept
{
  this->front_ = glm::normalize(glm::vec3{
      std::cos(glm::radians(this->yaw_)) * std::cos(glm::radians(this->pitch_)),
      std::sin(glm::radians(this->pitch_)),
      std::sin(glm::radians(this->yaw_))
          * std::cos(glm::radians(this->pitch_))});
  this->right_ = glm::normalize(glm::cross(this->front_, this->world_up_));
  this->up_ = glm::normalize(glm::cross(this->right_, this->front_));
  this->view_ =
      glm::lookAt(this->position_, this->position_ + this->front_, this->up_);
}
} // namespace redoom::graphics

namespace YAML
{
using redoom::graphics::Camera;

YAML::Emitter& operator<<(YAML::Emitter& out, const Camera& c)
{
  out << YAML::BeginMap;
  out << YAML::Key << "position" << YAML::Value << c.getPosition();
  out << YAML::Key << "yaw" << YAML::Value << c.getYaw();
  out << YAML::Key << "pitch" << YAML::Value << c.getPitch();
  out << YAML::Key << "speed" << YAML::Value << c.getSpeed();
  out << YAML::Key << "sensitivity" << YAML::Value << c.getSensitivity();
  out << YAML::Key << "fov" << YAML::Value << c.getFov();
  out << YAML::EndMap;
  return out;
}

Node convert<Camera>::encode(const Camera& rhs)
{
  Node node;
  node["position"] = rhs.getPosition();
  node["yaw"] = rhs.getYaw();
  node["pitch"] = rhs.getPitch();
  node["speed"] = rhs.getSpeed();
  node["sensitivity"] = rhs.getSensitivity();
  node["fov"] = rhs.getFov();
  return node;
}

bool convert<Camera>::decode(const Node& node, Camera& rhs)
{
  if (!node.IsMap() || node.size() != 6)
    return false;

  rhs.setPosition(node["position"].as<glm::vec3>());
  rhs.setYaw(node["yaw"].as<float>());
  rhs.setPitch(node["pitch"].as<float>());
  rhs.setSpeed(node["speed"].as<float>());
  rhs.setSensitivity(node["sensitivity"].as<float>());
  rhs.setFov(node["fov"].as<float>());
  return true;
}
} // namespace YAML
