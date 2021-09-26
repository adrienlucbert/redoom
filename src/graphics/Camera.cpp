#include <redoom/graphics/Camera.hh>

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
