#pragma once

#include <queue>
#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <tl/optional.hpp>

#include <redoom/physics/Fixture.hh>
#include <redoom/physics/Force.hh>

namespace redoom::graphics
{
class Model;
};

namespace redoom::physics
{
class World;

enum class BodyType { Static, Kinematic, Dynamic };

struct BodyTransformDefinition {
  glm::vec3 position{0.0f, 0.0f, 0.0f};
  float angle{0.0f};
  glm::vec3 rotation{1.0f, 1.0f, 1.0f};
  glm::vec3 scale{1.0f, 1.0f, 1.0f};
};

class BodyTransform
{
public:
  explicit BodyTransform(BodyTransformDefinition def = {}) noexcept;

  [[nodiscard]] glm::vec3 const& getPosition() const noexcept;
  void setPosition(glm::vec3 position) noexcept;
  [[nodiscard]] float getAngle() const noexcept;
  void setAngle(float angle) noexcept;
  [[nodiscard]] glm::vec3 const& getRotation() const noexcept;
  void setRotation(glm::vec3 rotation) noexcept;
  [[nodiscard]] glm::vec3 const& getScale() const noexcept;
  void setScale(glm::vec3 scale) noexcept;

  // Useful for the physics World to know this BodyTransform was updated since
  // last check. When called, resets the internal `is_updated` state to false.
  bool isUpdated() const noexcept;

private:
  glm::vec3 position_;
  float angle_;
  glm::vec3 rotation_;
  glm::vec3 scale_;
  mutable bool is_updated_{true};
};

struct BodyDefinition {
  BodyType type{BodyType::Dynamic};
  BodyTransform& transform;
  glm::vec3 linear_velocity{0.0f, 0.0f, 0.0f};
  float angular_velocity{0.0f};
  bool has_fixed_rotation{false};
  float gravity_scale{1.0f};
};

class Body
{
public:
  ~Body() noexcept = default;

  Body(Body const&) noexcept = delete;
  Body(Body&&) noexcept = default;

  Body& operator=(Body const&) noexcept = delete;
  Body& operator=(Body&&) noexcept = delete;

  Fixture& createFixture(FixtureDefinition def) noexcept;
  Fixture& createFixtureFromMesh(
      FixtureDefinition def, graphics::Mesh const& mesh) noexcept;

  void draw() const noexcept;

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
  [[nodiscard]] float getMass() const noexcept;
  void addForce(Force force) noexcept;
  void addConstantForce(Force force) noexcept;

private:
  Body(World& world, unsigned int id, BodyDefinition def) noexcept;
  void updateMass(Fixture const& fixture) noexcept;

  std::reference_wrapper<World> world_;
  unsigned int id_;
  BodyType type_;
  BodyTransform& transform_;
  glm::vec3 linear_velocity_;
  float angular_velocity_;
  bool has_fixed_rotation_;
  float gravity_scale_;
  float mass_{0.0f};
  std::vector<Fixture> fixtures_;
  std::queue<Force> forces_;
  std::vector<Force> constant_forces_;

  friend World;
};
} // namespace redoom::physics
