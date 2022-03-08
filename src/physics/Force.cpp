#include <redoom/physics/Force.hh>

namespace redoom::physics
{
glm::vec3 Force::compute(float mass, double timestep) const noexcept
{
  switch (this->type) {
    case Type::Force:
      if (mass == 0.0f)
        return glm::vec3{0.0f};
      return this->force * static_cast<float>(timestep) / mass;
    case Type::Acceleration: return this->force * static_cast<float>(timestep);
    case Type::Impulse:
      if (mass == 0.0f)
        return glm::vec3{0.0f};
      return this->force / mass;
    case Type::VelocityChange: return this->force;
  }
  assert(false && "Unsupported force type");
}
} // namespace redoom::physics
