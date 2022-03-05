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

class BodyTransform
{
public:
  BodyTransform(glm::vec3 position,
      float angle,
      glm::vec3 rotation,
      glm::vec3 scale) noexcept;

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
  glm::vec3 position_{0.0f, 0.0f, 0.0f};
  float angle_{0.0f};
  glm::vec3 rotation_{1.0f, 1.0f, 1.0f};
  glm::vec3 scale_{1.0f, 1.0f, 1.0f};
  mutable bool is_updated_{true};
};

struct BodyDefinition {
  BodyType type{BodyType::Static};
  BodyTransform& transform;
  glm::vec3 linear_velocity{0.0f, 0.0f, 0.0f};
  float angular_velocity{0.0f};
  bool has_fixed_rotation{false};
  float gravity_scale{1.0f};
};

class Body
{
public:
  Body(World& world, unsigned int id, BodyDefinition def) noexcept;
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
  std::reference_wrapper<World> world_;
  unsigned int id_;
  BodyType type_;
  BodyTransform& transform_;
  glm::vec3 linear_velocity_;
  float angular_velocity_;
  bool has_fixed_rotation_;
  float gravity_scale_;
  std::vector<Fixture> fixtures_;
};
} // namespace redoom::physics
