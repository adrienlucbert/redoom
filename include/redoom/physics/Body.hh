#pragma once

#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <tl/optional.hpp>

#include <redoom/physics/Fixture.hh>

namespace redoom::graphics
{
class Model;
};

namespace redoom::physics
{
class World;

enum class BodyType { Static, Kinematic, Dynamic };

struct BodyTransform {
  glm::vec3 position{0.0f, 0.0f, 0.0f};
  float angle{0.0f};
  glm::vec3 rotation{1.0f, 1.0f, 1.0f};
  glm::vec3 scale{1.0f, 1.0f, 1.0f};
};

struct BodyDefinition {
  BodyType type{BodyType::Static};
  BodyTransform transform;
  glm::vec3 linear_velocity{0.0f, 0.0f, 0.0f};
  float angular_velocity{0.0f};
  bool has_fixed_rotation{false};
  float gravity_scale{1.0f};
};

class Body
{
public:
  Body(World& pworld, unsigned int pid, BodyDefinition def) noexcept;
  ~Body() noexcept = default;

  Body(Body const&) noexcept = delete;
  Body(Body&& rhs) noexcept = default;

  Body& operator=(Body const&) noexcept = delete;
  Body& operator=(Body&&) noexcept = delete;

  Fixture& createFixture(FixtureDefinition def) noexcept;
  Fixture& createFixtureFromMesh(
      FixtureDefinition def, graphics::Mesh const& mesh) noexcept;

  void draw(graphics::Program& program) const noexcept;

  [[nodiscard]] World& getWorld() const noexcept;
  [[nodiscard]] unsigned int getId() const noexcept;
  [[nodiscard]] BodyType getType() const noexcept;
  [[nodiscard]] BodyTransform const& getTransform() const noexcept;
  [[nodiscard]] glm::vec3 const& getLinearVelocity() const noexcept;
  [[nodiscard]] float getAngularVelocity() const noexcept;
  [[nodiscard]] bool hasFixedRotation() const noexcept;
  [[nodiscard]] float getGravityScale() const noexcept;
  [[nodiscard]] std::vector<Fixture> const& getFixtures() const noexcept;
  [[nodiscard]] tl::optional<AABB> getGlobalAABB() const noexcept;
  [[nodiscard]] tl::optional<AABB> getLocalAABB() const noexcept;

private:
  std::reference_wrapper<World> world;
  unsigned int id;
  BodyType type;
  BodyTransform transform;
  glm::vec3 linear_velocity;
  float angular_velocity;
  bool has_fixed_rotation;
  float gravity_scale;
  std::vector<Fixture> fixtures;
};
} // namespace redoom::physics
