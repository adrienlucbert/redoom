#include <redoom/graphics/Camera.hh>

namespace redoom::graphics
{
Camera::Camera(
    glm::vec3 pposition, glm::vec3 pup, float pyaw, float ppitch) noexcept
  : position{pposition}
  , front{glm::vec3{0.0f, 0.0f, -1.0f}}
  , up{pup}
  , right{}
  , world_up{pup}
  , yaw{pyaw}
  , pitch{ppitch}
  , speed{0.1f}
  , sensitivity{0.3f}
  , fov{45.0f}
{
}

glm::vec3 Camera::getPosition() const noexcept
{
  return this->position;
}

void Camera::setPosition(glm::vec3 pposition) noexcept
{
  this->position = pposition;
}

glm::vec3 Camera::getFront() const noexcept
{
  if (this->needs_update)
    this->update();
  return this->front;
}

void Camera::setFront(glm::vec3 pfront) noexcept
{
  this->front = pfront;
  this->needs_update = true;
}

glm::vec3 Camera::getUp() const noexcept
{
  if (this->needs_update)
    this->update();
  return this->up;
}

void Camera::setUp(glm::vec3 pup) noexcept
{
  this->up = pup;
  this->needs_update = true;
}

glm::vec3 Camera::getRight() const noexcept
{
  if (this->needs_update)
    this->update();
  return this->right;
}

void Camera::setRight(glm::vec3 pright) noexcept
{
  this->right = pright;
  this->needs_update = true;
}

float Camera::getYaw() const noexcept
{
  return this->yaw;
}

void Camera::setYaw(float pyaw) noexcept
{
  this->yaw = pyaw;
  this->needs_update = true;
}

float Camera::getPitch() const noexcept
{
  return this->pitch;
}

void Camera::setPitch(float ppitch) noexcept
{
  this->pitch = ppitch;
  this->needs_update = true;
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
}

glm::mat4 Camera::getView() const noexcept
{
  if (this->needs_update)
    this->update();
  return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::update() const noexcept
{
  this->front = glm::normalize(glm::vec3{
      std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch)),
      std::sin(glm::radians(this->pitch)),
      std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch))});
  this->right = glm::normalize(glm::cross(this->front, this->world_up));
  this->up = glm::normalize(glm::cross(this->right, this->front));
  this->needs_update = false;
}
} // namespace redoom::graphics
