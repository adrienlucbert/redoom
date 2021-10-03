#include <redoom/graphics/Camera.hh>

#include <redoom/serializer/common.hh>

namespace redoom::graphics
{
Camera::Camera(glm::vec3 pposition) noexcept
  : position{pposition}
{
  this->updateView();
  this->updateProjection();
}

glm::vec3 const& Camera::getPosition() const noexcept
{
  return this->position;
}

void Camera::setPosition(glm::vec3 pposition) noexcept
{
  this->position = pposition;
  this->updateView();
}

glm::vec3 const& Camera::getFront() const noexcept
{
  return this->front;
}

void Camera::setFront(glm::vec3 pfront) noexcept
{
  this->front = pfront;
  this->updateView();
}

glm::vec3 const& Camera::getUp() const noexcept
{
  return this->up;
}

void Camera::setUp(glm::vec3 pup) noexcept
{
  this->up = pup;
  this->updateView();
}

glm::vec3 const& Camera::getRight() const noexcept
{
  return this->right;
}

void Camera::setRight(glm::vec3 pright) noexcept
{
  this->right = pright;
  this->updateView();
}

float Camera::getYaw() const noexcept
{
  return this->yaw;
}

void Camera::setYaw(float pyaw) noexcept
{
  this->yaw = pyaw;
  this->updateView();
}

float Camera::getPitch() const noexcept
{
  return this->pitch;
}

void Camera::setPitch(float ppitch) noexcept
{
  this->pitch = ppitch;
  this->updateView();
}

float Camera::getSpeed() const noexcept
{
  return this->speed;
}

void Camera::setSpeed(float pspeed) noexcept
{
  this->speed = pspeed;
}

float Camera::getSensitivity() const noexcept
{
  return this->sensitivity;
}

void Camera::setSensitivity(float psensitivity) noexcept
{
  this->sensitivity = psensitivity;
}

float Camera::getFov() const noexcept
{
  return this->fov;
}

void Camera::setFov(float pfov) noexcept
{
  this->fov = pfov;
  this->updateProjection();
}

void Camera::setViewportSize(int width, int height) noexcept
{
  assert(width > 0 && height > 0);
  this->aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
  this->updateProjection();
}

glm::mat4 const& Camera::getView() const noexcept
{
  return this->view;
}

glm::mat4 const& Camera::getProjection() const noexcept
{
  return this->projection;
}

void Camera::updateProjection() noexcept
{
  if (this->projection_type == ProjectionType::Perspective) {
    this->projection =
        glm::perspective(this->fov, this->aspect_ratio, 0.1f, 100.0f);
  } else
    // TODO(alucbert): add support for orthographic projection
    assert("Orthographic projection is not yet supported" == nullptr);
}

void Camera::updateView() noexcept
{
  this->front = glm::normalize(glm::vec3{
      std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch)),
      std::sin(glm::radians(this->pitch)),
      std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch))});
  this->right = glm::normalize(glm::cross(this->front, this->world_up));
  this->up = glm::normalize(glm::cross(this->right, this->front));
  this->view =
      glm::lookAt(this->position, this->position + this->front, this->up);
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
