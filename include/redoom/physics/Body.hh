#pragma once

#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include <redoom/physics/Fixture.hh>

namespace redoom::physics
{
class World;

class Body
{
public:
  enum class Type { Static, Kinematic, Dynamic };

  struct Definition {
    Type type{Type::Static};
    glm::vec2 position{0.0f, 0.0f};
    float angle{0.0f};
    glm::vec2 linear_velocity{0.0f, 0.0f};
    float angular_velocity{0.0f};
    bool has_fixed_rotation{false};
    float gravity_scale{1.0f};
  };

  Body(World& pworld, Definition def) noexcept;

private:
  World& world;
  Type type;
  glm::vec2 position;
  float angle;
  glm::vec2 linear_velocity;
  float angular_velocity;
  bool has_fixed_rotation;
  float gravity_scale;
  std::vector<Fixture> fixtures;
};
} // namespace redoom::physics
