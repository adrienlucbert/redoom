#pragma once

#include <memory>

#include <glm/gtc/type_ptr.hpp>

#include <redoom/physics/Shape.hh>

namespace redoom::graphics
{
class Mesh;
} // namespace redoom::graphics

namespace redoom::physics
{
class Body;

struct FixtureDefinition {
  std::unique_ptr<Shape> shape;
  glm::vec3 position{0.0f, 0.0f, 0.0f};
  float friction{0.2f};
  float restitution{0.0f};
  float density{0.0f};
};

class Fixture
{
public:
  Fixture(Body& pbody, FixtureDefinition def) noexcept;
  Fixture(Fixture const&) noexcept = delete;
  Fixture(Fixture&&) noexcept = default;
  ~Fixture() noexcept = default;

  Fixture& operator=(Fixture const&) noexcept = delete;
  Fixture& operator=(Fixture&&) noexcept = default;

  void draw(graphics::Program& program) const noexcept;

  [[nodiscard]] Body& getBody() const noexcept;
  [[nodiscard]] std::unique_ptr<Shape> const& getShape() const noexcept;
  [[nodiscard]] glm::vec3 const& getPosition() const noexcept;
  [[nodiscard]] float getFriction() const noexcept;
  [[nodiscard]] float getRestitution() const noexcept;
  [[nodiscard]] float getDensity() const noexcept;

  [[nodiscard]] static Fixture fromBounds(Body& pbody,
      FixtureDefinition def,
      glm::vec2 x_bounds,
      glm::vec2 y_bounds,
      glm::vec2 z_bounds) noexcept;
  [[nodiscard]] static Fixture fromMesh(
      Body& pbody, FixtureDefinition def, graphics::Mesh const& mesh) noexcept;

private:
  std::reference_wrapper<Body> body;
  std::unique_ptr<Shape> shape;
  glm::vec3 position;
  float friction;
  float restitution;
  float density;
};
} // namespace redoom::physics
