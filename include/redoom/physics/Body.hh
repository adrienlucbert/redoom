#pragma once

#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include <redoom/physics/Fixture.hh>

namespace redoom::graphics
{
class Model;
};

namespace redoom::physics
{
class World;

enum class BodyType { Static, Kinematic, Dynamic };

struct BodyDefinition {
  BodyType type{BodyType::Static};
  glm::vec3 position{0.0f, 0.0f, 0.0f};
  float angle{0.0f};
  glm::vec3 linear_velocity{0.0f, 0.0f, 0.0f};
  float angular_velocity{0.0f};
  bool has_fixed_rotation{false};
  float gravity_scale{1.0f};
};

class Body
{
public:
  Body(World& pworld, unsigned int pid, BodyDefinition def) noexcept;

  Fixture& createFixture(FixtureDefinition def) noexcept;
  Fixture& createFixtureFromMesh(
      FixtureDefinition def, graphics::Mesh const& mesh) noexcept;

  void draw(graphics::Program& program) const noexcept;

  [[nodiscard]] World& getWorld() const noexcept;
  [[nodiscard]] unsigned int getId() const noexcept;
  [[nodiscard]] BodyType getType() const noexcept;
  [[nodiscard]] glm::vec3 const& getPosition() const noexcept;
  [[nodiscard]] float getAngle() const noexcept;
  [[nodiscard]] glm::vec3 const& getLinearVelocity() const noexcept;
  [[nodiscard]] float getAngularVelocity() const noexcept;
  [[nodiscard]] bool hasFixedRotation() const noexcept;
  [[nodiscard]] float getGravityScale() const noexcept;
  [[nodiscard]] std::vector<Fixture> const& getFixtures() const noexcept;

private:
  std::reference_wrapper<World> world;
  unsigned int id;
  BodyType type;
  glm::vec3 position;
  float angle;
  glm::vec3 linear_velocity;
  float angular_velocity;
  bool has_fixed_rotation;
  float gravity_scale;
  std::vector<Fixture> fixtures;
};
} // namespace redoom::physics
